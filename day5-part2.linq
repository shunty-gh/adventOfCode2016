<Query Kind="Program">
  <Namespace>System.Security.Cryptography</Namespace>
</Query>

void Main()
{
	// Advent of code 2016
	// Day 5 - part 2
	// http://adventofcode.com/2016/day/5
    "Starting test ----------".Dump();
    var input = GetTestInput1();
	var result = GetResult1(input);
	result.Dump("Test Result 1");
    System.Diagnostics.Debug.Assert("05ace8e3" == result, "Test result is wrong.");

    "Starting live ----------".Dump();
    input = GetInput();
    result = GetResult1(input);
    result.Dump("Password");
}

public string GetResult1(string input)
{
    int index = 0;
    int found = 0;
    char[] result = new char[] { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    while (found < 8)
    {
        string hashed = GetHash($"{input}{index}");
        // Does it start with 5 zeroes
        if (hashed.StartsWith("00000"))
        {
            $"Hash - Index: {index}; Hash: {hashed}".Dump();
            char ch = hashed[5];
            int ci = ch - '0';
            if ((ci >= 0) && (ci <= 7) && (result[ci] == ' '))
            {
                result[ci] = hashed[6];
                found++;
                $"Hash accepted - Result: {string.Join("", result)}".Dump();
            }
        }
        index++;
    }
    return string.Join("", result).ToLower();
}

public MD5 md = MD5.Create();

public string GetHash(string input)
{
    var hash = md.ComputeHash(Encoding.UTF8.GetBytes(input));
    var str = string.Join("", hash.Select(b => b.ToString("X2")));
    return str;
}

public string GetTestInput1()
{
    return "abc";
}

public string GetInput()
{
    return "wtnhxymk";
}