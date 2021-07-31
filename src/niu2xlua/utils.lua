function table.tostring(tbl, indent, limit, depth, jstack)
    limit   = limit  or 1000
    depth   = depth  or 7
    jstack  = jstack or {name="top"}
    local i = 0

    local output = {}
    if type(tbl) == "table" then
    -- very important to avoid disgracing ourselves with circular referencs...
        for i,t in pairs(jstack) do
            if tbl == t then
                return "<" .. i .. ">,\n"
            end
        end
        jstack[jstack.name] = tbl

        table.insert(output, "{\n")

        local name = jstack.name
        for key, value in pairs(tbl) do
            local innerIndent = (indent or " ") .. (indent or " ")
            table.insert(output, innerIndent .. tostring(key) .. " = ")
            jstack.name = name .. "." .. tostring(key)
            table.insert(output,
                value == tbl and "<parent>," or table.tostring(value, innerIndent, limit, depth, jstack)
            )

            i = i + 1
            if i > limit then
                table.insert(output, (innerIndent or "") .. "...\n")
                break
            end
        end

        table.insert(output, indent and (indent or "") .. "},\n" or "}")
    else
        if type(tbl) == "string" then tbl = string.format("%q", tbl) end -- quote strings
        table.insert(output, tostring(tbl) .. ",\n")
    end

    return table.concat(output)
end