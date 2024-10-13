#include "luau/Ast/include/Luau/Ast.h"
#include "luau/Analysis/include/Luau/AstJsonEncoder.h"
#include "luau/Common/include/Luau/Common.h"
#include "Luau/Parser.h"
#include "luau/CLI/FileUtils.h"
#include "Luau/ToString.h"
#include <chrono>
#include <iostream>

using namespace std::chrono;

static int assertionHandler(const char *expr, const char *file, int line, const char *function)
{
    printf("%s(%d): ASSERTION FAILED: %s\n", file, line, expr);
    return 1;
}

int main(int argc, char **argv)
{
    Luau::assertHandler() = assertionHandler; // Set assertion handler

    std::optional<std::string> maybeSource = std::nullopt;
    maybeSource = readStdin();

    if (!maybeSource)
    {
        fprintf(stderr, "ERROR: Could not read STDIN\n");
        return 2;
    }

    std::string source = *maybeSource;

    Luau::Allocator allocator;
    Luau::AstNameTable names(allocator);

    Luau::ParseOptions options;
    options.captureComments = true;
    options.allowDeclarationSyntax = true;

    auto startTime = std::chrono::high_resolution_clock::now();

    Luau::ParseResult parseResult = Luau::Parser::parse(
        source.data(),
        source.size(),
        names,
        allocator,
        options);

    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(endTime - startTime);

    fprintf(stderr, "ParseStatus/DoneTime=%ldμs\n", duration.count());

    if (parseResult.errors.size() > 0)
    {
        fprintf(stderr, "ParseStatus/Error\nBlock:\n");
        for (const Luau::ParseError &error : parseResult.errors)
        {
            fprintf(stderr, "  %s - %s\n", toString(error.getLocation()).c_str(), error.getMessage().c_str());
        }
        fprintf(stderr, "EndBlock\n");
        return 3;
    }

    fprintf(stderr, "ParseDone/OK\n");

    bool noPrintAst = std::getenv("NO_PRINT_AST") != nullptr && strcmp(std::getenv("NO_PRINT_AST"), "1") == 0;

    if (!noPrintAst)
    {
        fprintf(stderr, "ParseStatus/RawAst=%s\n", Luau::toJson(parseResult.root).c_str());
        fprintf(stderr, "ParseStatus/RawAstWithComments=%s\n", Luau::toJson(parseResult.root, parseResult.commentLocations).c_str());
    }

    return 0;
}
