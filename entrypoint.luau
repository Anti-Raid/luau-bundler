@pragma {"lang":"lua"}
function (args) 
    --[[
        luau-bundler {{.Version}}
        Entry point for the bundle
    ]]--
    if 1==1 then
        {{.Body}}

        return main(args)
    end
end