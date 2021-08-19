local a = 'A'
for i = 1, 7 do
	print(i, a)
	a = pipe(a, 'hex')
end