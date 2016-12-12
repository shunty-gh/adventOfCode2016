<Query Kind="Program" />

//#define TEST

void Main()
{
	// Advent of code 2016
	// Day 7
	// http://adventofcode.com/2016/day/7
    var input = GetInput();
	var result = GetResult1(input);
	result.Dump("Test Result 1");
#if TEST	
    System.Diagnostics.Debug.Assert(2 == result, "Test result is wrong.");
#endif

//	input = GetInput();
//	result = GetResult1(input);
//	result.Dump("Result 1");
    
}

public int GetResult1(IEnumerable<string> input)
{
	int result = 0;
	foreach (var line in input)
	{
		if (SupportsTLS(line))
		{
			result++;
		}		
	}	
	return result;
}

public static string badpattern = @"\[[a-z]*(?<abba>(?<c1>[a-z])(?<c2>((?!\k<c1>)[a-z]))\k<c2>\k<c1>)[a-z]*\]";
public static string goodpattern = @"(?<c1>[a-z])(?<c2>((?!\k<c1>)[a-z]))\k<c2>\k<c1>";

public Regex bad_re = new Regex(badpattern);
public Regex good_re = new Regex(goodpattern);

public bool SupportsTLS(string input)
{
	var m = bad_re.Match(input);
	//m.Dump("Bad");
	if (m.Success)
		return false;
		
	m = good_re.Match(input);
	//m.Dump("Good");
	return m.Success;
}

public IEnumerable<string> GetInput()
{
#if TEST
	var result = new List<string>
	{
		"abba[mnop]qrst",
		"abcd[bddb]xyyx",
		"aaaa[qwer]tyui",
		"ioxxoj[asdfgh]zxcvbn"
	};
#else
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day7-input.txt");
	var result = File.ReadAllLines(fname);
#endif
	return result;
}