<Query Kind="Program" />

void Main()
{
	// Advent of code 2016
	// Day 2
	// http://adventofcode.com/2016/day/2
	
	var input = GetTestInput();
	var result = GetResult(input);
	result.Dump("Test input - part 1");

	input = GetFinalInput();
	result = GetResult(input);
	result.Dump("Main - part 1");

	input = GetTestInput();
	result = GetResult2(input);
	result.Dump("Test input - part 2");

	input = GetFinalInput();
	result = GetResult2(input);
	result.Dump("Main - part 2");
}

public string GetResult(IEnumerable<string> input)
{
	string result = "";
	var keypad = new KeypadNavigator();
	foreach (var s in input)
	{
		foreach (var c in s.ToCharArray())
		{
			keypad.Move(c);
		}
		result = result + keypad.KeypadDigit;
	}
	
	return result;
}

public string GetResult2(IEnumerable<string> input)
{
	string result = "";
	var keypad = new KeypadNavigator(KeypadType.Diamond);
	foreach (var s in input)
	{
		foreach (var c in s.ToCharArray())
		{
			keypad.Move(c);
		}
		result = result + keypad.KeypadDigit;
	}

	return result;
}

public IEnumerable<string> GetTestInput()
{
	var result = new List<string>
	{
		"ULL",
		"RRDDD",
		"LURDL",
		"UUUUD"
	};
	return result;
}

public IEnumerable<string> GetFinalInput()
{
	return new List<string> 
	{
		"LLLUDRDLLULDUDLLRLUDURULDURRRRLLURLDLDDDULLDDUDLRDLRDUURRDUUDLLRUUDDLULURDLRDUUDUDRURULLLLDRULDDLRDDRDLRDDLURLDDUDLLUUDLRDDDDLULUUURRDLUUDDLULLURRRDULLUDUDRDRDDRLDLLRLRDDDRDLULLUULDLLLRRDDUURUURDLLDRRDDLRULDLLDRLLUDRRDLUUULDLURLLDDURRUULLLLLRLUDLDDLLLURRRDUDULRULULDDLLDLRDDDUULRLRDUURLURRUDDURRUUDUDLDDLDULDDDDDULRULRDLRLDLRDLDDLUDRDUUDLDUDUDLLLRLUUDRUDDDRDRURRLLLDLUULDUULRDLULLUURLDRRRLDRLUDRRURLDULULDRUDDULLLDRDLLULUDDDDRDRULDULRRRRDDRULDLRDU",
		"DLDDRRDLLDUURRLULLLLRDRLUDURLDRRDURRRRUUDDUDRRDDDRRLDDLDDLURDLDRLUDULDUDUUDDDLLULRRLLUDULLLUULDDRDDUDUUDULURULULLDRLRUURDRDDLRRUDRUULLLLURRUDRDULDDRURRURLLLLLRLLLRLLUDUDLRDLULDUDLULLLUUDLLDDDUDUDLLRRDLRDLLLRRLRUDRDUDRURLUUURULLDDDDLLURDULURRLLLRLRRULLRRRLUUULLLLRLRDUURRDUURLLLDRDURRRULDDUDRRDLRLDLLRUDDLLUDRLLUUDRLLLLLLUDLURLLRUDRUUUULDLUDULLDDDDRLURUURDDDURRRLRLUDUUDURDDDDRRLRDLURDDLLULDRDDURLLURULUUUUURDUUULDRRLLURRRRRLDDUULLRULLDLDLDLRRRDRLDRUUD",
		"RLDRRRURULDLUDLDUDLLDUUURRDUDDURULLRRDDULUUDRRRULRUURRRLUUULRDRUDRRLLRLURDLDRDRDLLUDRUULRUDRUDDRURLRLURRDDRRURUUDRRDDRURURUDUUUDUDRRLRDRUUURLLUUUDLRUUDDRDDDDLDRLRDUDDULDDLRLLRURLLURLDDLDLDDULLDDUUURDLRUDUUDLDURDDRUULDRDDRDDDUUUDRDRDDRRDRRDLRDRURDUDDLUUUDULLUULULULRDRUUDDURURDRRRRLUDLDUDURLDRURDLLUUUDLRRDRRURDDULULURLDUDDLUDLDDLLRLDULLULULURUURLDULUDLLUUDLDDULDRRDDUULLUDLDLLRDRDURDDURDDURLDDURUURLLRURURUDDURRDRLRLDDUUDUULRDLLURRRRULURULDUDUDDUDDRLLLDLURDUURUURLUULRRLDLULDDRLDDUURULURUDRD",
		"URLDDRLLRRLDRLLRRURURURDDLRRRUUUURULRRUUDLUDRULLDLRUDDLULRUULDULURLLRLLUDDUDLURDRRRRLURULRURRURRULRRRULDLLDDLRLUDULUUUDDUDDRRDDDDUULRRLDRRULULRDUURRLDDRDULDURUDUDDLDLLURDDLDDRUDUUUDUUURDLDUDUUULLDLRDULRRRDLLURLDLLULRDDULULURLRLUULRLLLDDDUDLLDLURRRULRDUDDLULUDRUDDURULRLRUDDURLLURULLURDRULDUDLDULRRDLDURLUURRDDUDDUDRURUDDURRUUDURUULLLLDDRDDDDDULUUDDURRULLDRRLRRRRRDDRUUDDDURDRDRUDDUULDUDRRDRULUURLURLUDUDULDDRDULDLRUUDLLLRRLRDRDDUUULRDUDLUDLURRDUURDULDRLLDRDULDUDUULRLLDLRLDLUUDLRUULDUUULDLRLRLUULLLLRLRDUDRUUDURLDUDRRURLRUDRRLRDDLRDDLDDUDDDRLRLLRLUUURLURRRLULRLLDRLRDDRRDRL",
		"DLLLLLLRLRDRUDLRLLRLDLRURRUURLDLDDDDDUDUULLLLRRLRRDUUDUDLULLRRDULUDLLULURLRULURUULRLURDUDLUDULULUUURLRUDULURULRURULURLRLDRRRRLUDLLDULLDDLLULUURRULRDURDUUDDDURRUDLLLLRLDLUDDULLDUDDURURURRRRULDULULUDDUUDRLRLLLDLLLUUUURUDUUDLDLLRLRDDUULLUURLDDLRRDRLULDLULRULDLDURLULUURRRUDLLRDLUDDULRULULUDDURDLUUURDUUURDUDURLUUDRLUDRULUDDRRDLUUDLLLRDDDDDDLDURDDLDRDLUUDRULLUDRDLDULLULDDRUUDRRLRURRUULLRLRDUUURRDRRDULDDULUUDDURLULRLRURLLRRR",
	};
}

public enum KeypadType
{
	ThreeByThree,
	Diamond,
}

public class KeypadNavigator
{
	private int x = 5;

	private KeypadType keypadType = KeypadType.ThreeByThree;
	private int[] topBoundary;
	private int[] leftBoundary;
	private int[] bottomBoundary;
	private int[] rightBoundary;

	public KeypadNavigator(KeypadType padType = KeypadType.ThreeByThree)
	{
		CreateBoundaries(padType);
	}

	private void CreateBoundaries(KeypadType padType)
	{
		this.keypadType = padType;
		switch (keypadType)
		{
			case KeypadType.ThreeByThree:
				// Default 3x3 grid
				topBoundary = new int[] { 1, 2, 3 };
				leftBoundary = new int[] { 1, 4, 7 };
				bottomBoundary = new int[] { 7, 8, 9 };
				rightBoundary = new int[] { 3, 6, 9 };
				break;
			case KeypadType.Diamond:
				// Diamond shape
				topBoundary = new int[] { 1, 2, 4, 5, 9 };
				leftBoundary = new int[] { 1, 2, 5, 10, 13 };
				bottomBoundary = new int[] { 5, 9, 10, 12, 13 };
				rightBoundary = new int[] { 1, 4, 9, 12, 13 };
				break;
			default:
				break;
		}
	}

	public void Move(char instruction)
	{
		switch (instruction)
		{
			case 'U':
				if (!topBoundary.Contains(x))
				{
					if (keypadType == KeypadType.Diamond)
					{
						if (x == 3 || x == 13)
						{
							x -= 2;
						}
						else
						{
							x -= 4;
						}
					}
					else
					{
						x -= 3;
					}
				}
				break;
			case 'D':
				if (!bottomBoundary.Contains(x))
				{
					if (keypadType == KeypadType.Diamond)
					{
						if (x == 1 || x == 11)
						{
							x += 2;
						}
						else
						{
							x += 4;
						}
					}
					else
					{
						x += 3;
					}
				}
				break;
			case 'L':
				if (!leftBoundary.Contains(x))
				{
					x -= 1;
				}
				break;
			case 'R':
				if (!rightBoundary.Contains(x))
				{
					x += 1;
				}
				break;
			default:
				break;
		}
		//$"Moved {instruction}. New location x={x}".Dump();
	}

	public string KeypadDigit { get { return string.Format("{0:x}", x).ToUpper(); } }
}
