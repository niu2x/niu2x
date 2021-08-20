local old_pipe = pipe
local pipe_digest = pipe_digest

local digest_algorithms = {
	'md5',
	'sha256',
}

function pipe(s, f)
	local filters = string.split(f, '|')
	for _, v in ipairs(filters) do 
		if table.include(digest_algorithms, v) then
			s = pipe_digest(s, v)
		else
			s = old_pipe(s, filter[v])
		end
	end
	return s
end