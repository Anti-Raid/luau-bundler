function main(args)
    local double = import("double.luau")

    local a = 1
    for i, v in ipairs(args) do
        a += 1
    end

    return double.double(a)
end