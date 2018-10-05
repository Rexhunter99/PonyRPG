
-- Set the random generator seed
math.randomseed(os.time());

-- Print out the Lua version
io.write(string.format("%s\n", _VERSION));

local counter = 5;

while counter > 0 do
	io.write(string.format("Countdown: %d\n", counter));
	counter = counter - 1;
end
