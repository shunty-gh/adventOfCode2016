<Query Kind="Program" />

//#define TEST

void Main()
{
	// Advent of code 2016
	// Day 7
	// http://adventofcode.com/2016/day/7
    var input = GetInput();
	var result = GetResult(input);
	result.Dump("Result 1");
#if TEST	
    System.Diagnostics.Debug.Assert(2 == result, "Test result is wrong.");
#endif

}

public Tuple<int, int> GetResult(IEnumerable<string> input)
{
	int resultTLS = 0, resultSSL = 0;
	foreach (var line in input)
	{
		if (SupportsTLS(line))
		{
			resultTLS++;
		}
		if (SupportsSSL(line))
		{
			resultSSL++;
		}
	}
	return new Tuple<int, int>(resultTLS, resultSSL);
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

public bool SupportsSSL(string input)
{
	var abas = new List<string>();
	var hypernets = new List<string>();

	Action<string> GetABAs = (str) =>
	{
		int index = 0;
		char c1, c2, c3;
		while (index < str.Length - 2)
		{
			c1 = str[index++];
			c2 = str[index];
			c3 = str[index + 1];
			if ((c1 == c3) && (c1 != c2))
			{
				abas.Add($"{c1}{c2}{c3}");
			}
		}
	};

	Action<string> SplitInput = (str) =>
	{
		string current = "";
		foreach (char c in str)
		{
			switch (c)
			{
				case '[':
					if (!string.IsNullOrWhiteSpace(current))
					{
						GetABAs(current);
						current = "";
					}
					break;
				case ']':
					if (!string.IsNullOrWhiteSpace(current))
					{
						hypernets.Add(current);
						current = "";
					}
					break;
				default:
					current += c;
					break;
			}
		}
		// Assume [ and ] are balanced - therefore must be a 'supernet' sequence
		if (!string.IsNullOrWhiteSpace(current))
		{
			GetABAs(current);
		}
	};

	SplitInput(input);
	var hyper_str = string.Join("|", hypernets);

	foreach (var aba in abas)
	{
		string bab = $"{aba[1]}{aba[0]}{aba[1]}";
		if (hyper_str.Contains(bab))
		{
			return true;
		}
	}
	return false;
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