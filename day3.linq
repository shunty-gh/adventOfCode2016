<Query Kind="Program" />

void Main()
{
	// Advent of code 2016
	// Day 3
	// http://adventofcode.com/2016/day/3
	
	var input = GetInput();
	var result = GetResult1(input);
	result.Dump("Result 1");

	result = GetResult2(input);
	result.Dump("Result2 ");
}

public int GetResult1(IEnumerable<string> input)
{
	int result = 0;
	foreach (var line in input)
	{
		// Split the input line into three lengths
		var sides = ParseInputLine(line);
		// Check them
		if (IsValid(sides))
        {
			result++;
		}
	}
	return result;
}

public int GetResult2(IEnumerable<string> input)
{
	int result = 0;
	var allsides = ParseInputByColumn(input);
	foreach (var sides in allsides)
	{
		if (IsValid(sides))
		{
			result++;
		}
	}
	return result;
}

public IEnumerable<string> GetInput()
{
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day3-input.txt");
	var result = File.ReadAllLines(fname);
	return result;
}

public Tuple<int, int, int> ParseInputLine(string line)
{
	// Input line is approximately of the form "  nnn  nnn nnn"
	var split = line.Split(' ')
		.Where(s => !string.IsNullOrWhiteSpace(s))
		.Select(s => int.Parse(s.Trim()))
		.ToArray();	
	return new Tuple<int, int, int>(split[0], split[1], split[2]);
}

public IEnumerable<Tuple<int, int, int>> ParseInputByColumn(IEnumerable<string> input)
{
	var allsides = new List<int>();
	var allsides2 = new List<int>();
	var allsides3 = new List<int>();
	foreach (var line in input)
	{
		var sides = ParseInputLine(line);
		allsides.Add(sides.Item1);
		allsides2.Add(sides.Item2);
		allsides3.Add(sides.Item3);
	}
	allsides.AddRange(allsides2);
	allsides.AddRange(allsides3);
	
	var result = new List<Tuple<int, int, int>>();
	int index = 0;
	int count = allsides.Count;
	var sidesarray = allsides.ToArray();
	// Should really put in error checking but assume that we have a valid input with (count mod 3 = 0) number of items.
	while (index < count)
	{
		var side = new Tuple<int, int, int>(sidesarray[index], sidesarray[index + 1], sidesarray[index + 2]);
		result.Add(side);
		index += 3;
	}
	
	return result;
}

public bool IsValid(Tuple<int, int, int> sides)
{
	// 3 combinations possible
	var result = ((sides.Item1 + sides.Item2) > sides.Item3)
		&& ((sides.Item1 + sides.Item3) > sides.Item2)
		&& ((sides.Item2 + sides.Item3) > sides.Item1);
		
	return result;
}