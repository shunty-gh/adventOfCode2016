<Query Kind="Program" />

//#define TEST
//#define TEST2

void Main()
{
	// Advent of code 2016
	// Day 9
	// http://adventofcode.com/2016/day/9
    var input = GetInput();
	var result = GetResult(input);
	result.Dump("Result 1");
#if TEST	
    System.Diagnostics.Debug.Assert(57 == result, "Test result is wrong.");
#elif TEST2
    System.Diagnostics.Debug.Assert(242394 == result, "Test result is wrong.");
#endif
}

public Int64 GetResult(IEnumerable<string> input)
{
    bool usev2 = true;
#if TEST
    usev2 = false;
    var expects = new List<int> { 6, 7, 9, 11, 6, 18 };
    int index = 0;
#elif TEST2
    var expects = new List<int> { 9, 20, 241920, 445 };
    int index = 0;
#endif
    Int64 result = 0;
	foreach (var line in input)
	{
        var dl = DecompressChunk(line, usev2);
        result += dl;
#if TEST || TEST2
        var expect = expects[index++];
        var r = dl;
        System.Diagnostics.Debug.Assert(expect == r, $"Test result is wrong. Expected \"{expect}\" but got \"{r}\"");
#endif
    }
    return result;
}

public static string marker_pattern = @"\((?<len>[0-9]+)x(?<rep>[0-9]+)\)";
public Regex re = new Regex(marker_pattern);

public Int64 DecompressChunk(string source, bool useV2)
{
    Int64 result = 0;
    int index = 0;
    var match = re.Match(source, index);
    while (match.Success)
    {
        if (match.Index > 0)
        {
            result += match.Index - index;
        }

        int len = int.Parse(match.Groups["len"].Value);
        int rep = int.Parse(match.Groups["rep"].Value);
        var sub = source.Substring(match.Index + match.Length, len);
        if (useV2)
        {
            Int64 r = DecompressChunk(sub, true);
            result += r * rep;
        }
        else
        {
            result += (sub.Length * rep);
        }
        
        index = match.Index + match.Length + len;
        if (index >= source.Length)
            break;
        match = re.Match(source, index);
    }

    result += source.Length - index;
    return result;
}

public IEnumerable<string> GetInput()
{
#if TEST
	var result = new List<string>
	{
		"ADVENT",
        "A(1x5)BC",
		"(3x3)XYZ",
		"A(2x2)BCD(2x2)EFG",
		"(6x1)(1x3)A",
        "X(8x2)(3x3)ABCY"
	};
#elif TEST2
    var result = new List<string>
    {
        "(3x3)XYZ",
        "X(8x2)(3x3)ABCY",
        "(27x12)(20x12)(13x14)(7x10)(1x12)A",
        "(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN"
    };
#else 
	string fname = Path.Combine(
        Path.GetDirectoryName(Util.CurrentQueryPath), 
        Path.GetFileNameWithoutExtension(Util.CurrentQueryPath) + "-input.txt");
        
	var result = File.ReadAllLines(fname);
#endif
	return result;
}