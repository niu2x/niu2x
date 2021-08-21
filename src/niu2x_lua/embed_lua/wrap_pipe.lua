local old_pipe = pipe
local pipe_digest = pipe_digest

local digest_algorithms = {
	'blake2b512',
	'blake2s256',

	'md5',
	'md5-sha1',

	'sha1',

	'sha224',
	'sha256',
	'sha512-224',
	'sha512-256',
	'sha384',
	'sha512',

	'sha3-224',
	'sha3-256',
	'sha3-384',
	'sha3-512',
	'sha3-224',
	'shake128',
	'shake256',

	'sm3',
}

local pipe = {
	digest_algorithms = digest_algorithms,
}

setmetatable(pipe, {
	__call = function(t, s, f)
		local filters = string.split(f, '|')
		for _, v in ipairs(filters) do 
			if table.include(digest_algorithms, v) then
				s = pipe_digest(s, v)
			else
				s = old_pipe(s, filter[v])
			end
		end
		return s
	end,
})

_G.pipe = pipe