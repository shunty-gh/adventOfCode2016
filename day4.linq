<Query Kind="Program" />

void Main()
{
	// Advent of code 2016
	// Day 4
	// http://adventofcode.com/2016/day/4
    var input = GetTestInput1();
	var result = GetResult1(input);
	result.Dump("Test Result 1");
    System.Diagnostics.Debug.Assert(1514 == result, "Test result is wrong.");

	input = GetInput();
	result = GetResult1(input);
	result.Dump("Result 1");
    
    input = GetTestInput2();
    var roomnames = GetRoomNames(input);
    roomnames.Dump();

	input = GetInput();
	roomnames = GetRoomNames(input);
	roomnames.Where(n => n.Item1.Contains("northpole")).Select(n => new { RoomName = n.Item1, SectorId = n.Item2 }).Dump();
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

public IEnumerable<Tuple<string, int>> GetRoomNames(IEnumerable<string> input)
{
	var result = new List<Tuple<string, int>>();
	foreach (var line in input)
	{
		var parsed = ParseInputLine(line);
		var cs = CalculateChecksum(parsed.Item1);
		if (cs == parsed.Item2)
		{
			var decoded = DecryptRoomName(parsed.Item1, parsed.Item3);
            result.Add(new Tuple<string, int>(decoded, parsed.Item3));
        }
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
    var parsed = ParseInputLine(input);
    var cs = CalculateChecksum(parsed.Item1);
    return (cs == parsed.Item2) ? parsed.Item3 : 0;
}

public Tuple<string, string, int> ParseInputLine(string input)
{
	var match = sector_re.Match(input);
	var sector = int.Parse(match.Groups["sector"].Value);
	var hash = match.Groups["hash"].Value;
	var encr = match.Groups["encr"].Value;
	//$"encr: {encr}; sector: {sector}; hash: {hash}".Dump();

	return new Tuple<string, string, int>(encr, hash, sector);
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

public string DecryptRoomName(string encrypted, int sectorId)
{
	var words = new List<string>();
	var split = encrypted.Split('-');
	foreach (var chunk in split)
	{
    	var word = DecryptRoomWord(chunk, sectorId);
        words.Add(word);
	}
    return string.Join(" ", words);
}

public string DecryptRoomWord(string chunk, int sectorId)
{
	var chars = new List<char>();
	foreach (var c in chunk)
	{
		var result = (char)(((((int)(c) - (int)('a')) + sectorId) % 26) + (int)('a'));
		chars.Add(result);
	}
	return string.Join("", chars);
}

public IEnumerable<string> GetTestInput1()
{
	return new List<string>
	{
        "aaaaa-bbb-z-y-x-123[abxyz]",
        "a-b-c-d-e-f-g-h-987[abcde]",
        "not-a-real-room-404[oarel]",
        "totally-real-room-200[decoy]",
	};
}

public IEnumerable<string> GetTestInput2()
{
	return new List<string>
	{
		"qzmt-zixmtkozy-ivhz-343[zimth]",
	};
}

public IEnumerable<string> GetInput()
{
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day4-input.txt");
	var result = File.ReadAllLines(fname);
	return result;
}

