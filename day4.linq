<Query Kind="Program" />

void Main()
{
	// Advent of code 2016
	// Day 4
	// http://adventofcode.com/2016/day/4
    var input = GetTestInput();
	var result = GetResult1(input);
	result.Dump("Test Result 1");
    System.Diagnostics.Debug.Assert(1514 == result, "Test result is wrong.");

	input = GetInput();
	result = GetResult1(input);
	result.Dump("Result 1");
}

public int GetResult1(IEnumerable<string> input)
{
	int result = 0;
	foreach (var line in input)
	{
		int sid = GetSectorId(line);
        result += sid;
    }
    return result;
}

public static string sector_re_pattern = @"(?<encr>.*)-(?<sector>[0-9]+)\[(?<hash>[a-z]+)\]";
public Regex sector_re = new Regex(sector_re_pattern);

/// <summary>
/// If the input is a valid room then return the sector id otherwise return 0
/// </summary>
public int GetSectorId(string input)
{
	// Input is of the form "xxxx-yyyy-zz-aaa....-999[xyz]"
    var match = sector_re.Match(input);
	var sector = int.Parse(match.Groups["sector"].Value);
	var hash = match.Groups["hash"].Value;
	var encr = match.Groups["encr"].Value;
	//$"encr: {encr}; sector: {sector}; hash: {hash}".Dump();
    var cs = CalculateChecksum(encr);
    return (cs == hash) ? sector : 0;
}

public string CalculateChecksum(string encrypted)
{
	var src = encrypted.Replace("-", "");
	var result = src.ToCharArray()
        .GroupBy(c => c)
        .Select(g => new { Key = g.Key, Count = g.Count() })
        .OrderByDescending(g => g.Count)
        .ThenBy(g => g.Key)
        .Take(5)
        .Select(g => g.Key);

	return string.Join("", result);
}

public IEnumerable<string> GetTestInput()
{
	return new List<string>
	{
        "aaaaa-bbb-z-y-x-123[abxyz]",
        "a-b-c-d-e-f-g-h-987[abcde]",
        "not-a-real-room-404[oarel]",
        "totally-real-room-200[decoy]",
	};
}

public IEnumerable<string> GetInput()
{
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day4-input.txt");
	var result = File.ReadAllLines(fname);
	return result;
}

