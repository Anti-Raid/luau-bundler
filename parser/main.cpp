#include "luau/Ast/include/Luau/Ast.h"
#include "luau/Analysis/include/Luau/AstJsonEncoder.h"
#include "luau/Common/include/Luau/Common.h"
#include "Luau/Parser.h"
#include "luau/CLI/FileUtils.h"
#include "Luau/ToString.h"

#include <iostream>

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

    Luau::ParseResult parseResult = Luau::Parser::parse(
        source.data(),
        source.size(),
        names,
        allocator,
        options);

    if (parseResult.errors.size() > 0)
    {
        fprintf(stderr, "ParseDone/ERROR:\n");
        for (const Luau::ParseError &error : parseResult.errors)
        {
            fprintf(stderr, "  %s - %s\n", toString(error.getLocation()).c_str(), error.getMessage().c_str());
        }
        fprintf(stderr, "\n");
        return 3;
    }

    fprintf(stderr, "ParseDone/OK\n");

    fprintf(stderr, "%s", Luau::toJson(parseResult.root, parseResult.commentLocations).c_str());
}