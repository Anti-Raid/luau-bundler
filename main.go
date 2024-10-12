package main

import (
	_ "embed"
	"flag"
	"fmt"
	"os"
	"path/filepath"

	"github.com/infinitybotlist/eureka/crypto"
)

//go:embed entrypoint.luau
var entrypoint string

func main() {
	var dir *string
	var outputFile *string

	dir = flag.String("dir", "", "The directory to use")
	outputFile = flag.String("output", "output.luac", "The output file to write to")

	flag.Parse()

	if dir == nil || *dir == "" {
		panic("dir is required")
	}

	if outputFile == nil || *outputFile == "" {
		panic("output is required")
	}

	var bundleMap = make(map[string]*BundledFile)

	var queue = make(chan string, 100)

	queue <- *dir

	count := 0
	for filename := range queue {
		fmt.Printf("[%d/%d] %s", count, len(queue), filename)

		st, err := os.Stat(filename)

		if err != nil {
			panic(err)
		}

		if st.IsDir() {
			fmt.Printf(" (directory)\n")

			// Add all the files in the directory to the queue
			files, err := os.ReadDir(filename)

			if err != nil {
				panic(err)
			}

			for _, file := range files {
				queue <- filepath.Join(filename, file.Name())
			}

			// Close queue if done
			if len(queue) == 0 {
				close(queue)
			}

			continue
		}

		bundledFile, err := AddFileToBundle(filename)

		if err != nil {
			panic(err)
		}

		// Add to bundleMap
		bundleMap[filename] = bundledFile

		count++

		// Close queue if done
		if len(queue) == 0 {
			close(queue)
		}
	}

	fmt.Println("Bundling files...", bundleMap)
}

type BundledFile struct {
	ID       string
	FileName string
	Content  string
}

// Adds a file to the bundle set
//
// Note: this only reads files and marks them with unique IDs
func AddFileToBundle(file string) (*BundledFile, error) {
	content, err := os.ReadFile(file)

	if err != nil {
		return nil, err
	}

	// Generate random ID for the file
	bundledFile := &BundledFile{
		ID:       crypto.RandString(10),
		FileName: file,
		Content:  string(content),
	}

	return bundledFile, nil
}
