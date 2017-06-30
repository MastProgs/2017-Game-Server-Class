function plustwo (x)
	local a;
	a = 20
	return x + a;
end

rows = 6;
cols = plustwo(rows);

function addnum_lua (a, b)
	return c_addnum(a, b); -- c언어의 이 함수를 호출해서 빠른 연산처리를 C로 넘긴다.
end