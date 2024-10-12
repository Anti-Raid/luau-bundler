# Luau Bundler

Bundles a set of Luau files into a template file. This also gets around the following limitations of Anti-Raid Luau templates:

- The template must be a single string.
- The entrypoint of any Lua template is ``function(args)``. All code must be inside this function.
- Naked returns are not allowed within a function (outside the last return) are not allowed. E.g, the below may not compile:
If you need this behaviour, consider using [luau-bundler](https://www.github.com/Anti-Raid/luau-bundler).
