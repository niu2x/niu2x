for k, v in pairs(nx.lua_bindings) do
	nx[k] = v
end
nx.lua_bindings = nil
