local old_pipe = pipe

function pipe(s, f)
	local filters = string.split(f, '|')
	for _, v in ipairs(filters) do 
		s = old_pipe(s, filter[v])
	end
	return s
end