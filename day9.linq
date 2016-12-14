<Query Kind="Program" />

//#define TEST

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
#endif
}

public int GetResult(IEnumerable<string> input)
{
#if TEST
    var expects = new List<string> { "ADVENT", "ABBBBBC", "XYZXYZXYZ", "ABCBCDEFEFG", "(1x3)A", "X(3x3)ABC(3x3)ABCY" };
    int index = 0;
#endif
    int result = 0;
	foreach (var line in input)
	{
        string dl = DecompressLine(line);
        result += dl.Length;
#if TEST
        var expect = expects[index++];
        dl.Dump();
        System.Diagnostics.Debug.Assert(expect == dl, $"Test result is wrong. Expected \"{expect}\" but got \"{dl}\"");
#endif
    }
    return result;
}

public static string marker_pattern = @"\((?<len>[0-9]+)x(?<rep>[0-9]+)\)";
public Regex re = new Regex(marker_pattern);

public string DecompressLine(string source)
{
    StringBuilder sb = new StringBuilder();
    int index = 0;
    var match = re.Match(source, index);
    while (match.Success)
    {
        //match.Dump();
        if (match.Index > 0)
        {
            sb.Append(source.Substring(index, match.Index - index));
        }

        int len = int.Parse(match.Groups["len"].Value);
        int rep = int.Parse(match.Groups["rep"].Value);
        var sub = source.Substring(match.Index + match.Length, len);
        for (int subcount = 0; subcount < rep; subcount++)
        {
            sb.Append(sub);
        }
        
        index = match.Index + match.Length + len;
        if (index >= source.Length)
            break;
        match = re.Match(source, index);
    }

    if (index < source.Length)
        sb.Append(source.Substring(index));

    return sb.ToString();
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
#else
	string fname = Path.Combine(
        Path.GetDirectoryName(Util.CurrentQueryPath), 
        Path.GetFileNameWithoutExtension(Util.CurrentQueryPath) + "-input.txt");
        
	var result = File.ReadAllLines(fname);
#endif
	return result;
}