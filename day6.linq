<Query Kind="Program" />

void Main()
{
	// Advent of code 2016
	// Day 6
	// http://adventofcode.com/2016/day/6
    var input = GetTestInput1();
	var result = GetResult1(input);
	result.Dump("Test Result 1");
    System.Diagnostics.Debug.Assert("easter" == result, "Test result is wrong.");

	input = GetInput();
	result = GetResult1(input);
	result.Dump("Result 1");
    
}

public string GetResult1(IEnumerable<string> input)
{
    var cols = new List<List<char>>();
    foreach (var line in input)
    {
        int chindex = 0;
        foreach (var ch in line)
        {
            if (cols.Count < chindex + 1)
            {
                cols.Add(new List<char>());
            }

            var col = cols[chindex];
            col.Add(ch);
            chindex++;
        }
    }

    var chars = new List<char>();
    foreach (var col in cols)
    {
        var ch = col.GroupBy(c => c).OrderBy(g => g.Count()).Select(g => new { Key = g.Key, Count = g.Count() }).First().Key;
        //ch.Dump();
        chars.Add(ch);
    }
    
    return string.Join("", chars);
}

public IEnumerable<string> GetTestInput1()
{
	return new List<string>
    {
        "eedadn",
        "drvtee",
        "eandsr",
        "raavrd",
        "atevrs",
        "tsrnev",
        "sdttsa",
        "rasrtv",
        "nssdts",
        "ntnada",
        "svetve",
        "tesnvt",
        "vntsnd",
        "vrdear",
        "dvrsen",
        "enarar",
    };
}

public IEnumerable<string> GetInput()
{
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day6-input.txt");
	var result = File.ReadAllLines(fname);
	return result;
}

