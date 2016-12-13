<Query Kind="Program" />

//#define TEST

void Main()
{
	// Advent of code 2016
	// Day 8
	// http://adventofcode.com/2016/day/8
    var input = GetInput();
	var result = GetResult(input);
	result.Dump("Result 1");
#if TEST	
    System.Diagnostics.Debug.Assert(6 == result, "Test result is wrong.");
#endif

}

public enum AoCCommandType
{
	Unknown,
	Rect,
	RotateRow,
	RotateCol,
}

public int GetResult(IEnumerable<string> input)
{
#if TEST
    var grid = new AoCGrid(7,3);
#else
    var grid = new AoCGrid(50, 6);
#endif
	foreach (var line in input)
	{
		var command = ParseCommand(line);
		grid.ApplyCommand(command);
	}
	grid.DrawGrid();
	return grid.HowManyLit;
}

public class AoCCommand
{
	public AoCCommandType CommandType { get; set; }
	public int Item1 { get; set; }
	public int Item2 { get; set; }
}

public AoCCommand ParseCommand(string input)
{
	var split = input.Split(' ');
	AoCCommandType ct = AoCCommandType.Unknown;
	Tuple<int,int> parms = null;
	switch (split[0])
	{
		case "rect":
			ct = AoCCommandType.Rect;
			parms = GetRectParams(split[1]);
			break;
		case "rotate":
			ct = split[1] == "row" ? AoCCommandType.RotateRow : AoCCommandType.RotateCol;
			parms = GetRotateParams(split[2], split[4]);
			break;
	}
	var result = new AoCCommand
	{
		CommandType = ct,
		Item1 = parms.Item1,
		Item2 = parms.Item2
	};
	return result;
}

public Tuple<int, int> GetRectParams(string paramstr)
{
	var split = paramstr.Split('x');
	return new Tuple<int, int>(int.Parse(split[0]), int.Parse(split[1]));
}

public Tuple<int, int> GetRotateParams(string p1, string p2)
{
	return new Tuple<int, int>(int.Parse(p1.Substring(2)), int.Parse(p2));
}

public class AoCGrid
{
	private int _width, _height;
	private bool[] cells;
	public AoCGrid(int width, int height)
	{
		_width = width;
		_height = height;
		cells = new bool[width * height];
	}

	public void ApplyCommand(AoCCommand command)
	{
		command.Dump();
		switch (command.CommandType)
		{
			case AoCCommandType.Rect:
				FillRect(command.Item1, command.Item2);
				break;
			case AoCCommandType.RotateRow:
				RotateRow(command.Item1, command.Item2);
                break;
            case AoCCommandType.RotateCol:
                RotateCol(command.Item1, command.Item2);
                break;
            default:
				break;
		}
	}

	public void FillRect(int width, int height)
	{
		for (int rowindex = 0; rowindex < height; rowindex++)
		{
            int rowstart = rowindex * _width;
			for (var colindex = 0; colindex < width; colindex++)
			{
				cells[rowstart + colindex] = true;
			}
		}
	}
	
	private bool[] GetRow(int rowindex)
	{
        var result = new bool[_width];
        int rowstart = rowindex * _width;
        for (int colindex = 0; colindex < _width; colindex++)
		{
			result[colindex] = cells[rowstart + colindex];
		}
		return result;
	}

	private void SetRow(int rowindex, bool[] newrow)
    {
        int rowstart = rowindex * _width;
        for (int colindex = 0; colindex < _width; colindex++)
		{
			cells[rowstart + colindex] = newrow[colindex];
		}
	}

    private bool[] GetCol(int colindex)
    {
        var result = new bool[_height];
        int index = colindex;
        for (int rowindex = 0; rowindex < _height; rowindex++)
        {
            result[rowindex] = cells[index];
            index += _width;
        }
        return result;
    }

    private void SetCol(int colindex, bool[] newcol)
    {
        int index = colindex;
        for (int rowindex = 0; rowindex < newcol.Length; rowindex++)
        {
            cells[index] = newcol[rowindex];
            index += _width;
        }
    }
    
	private bool[] RotateBlock(bool[] block, int amount)
	{
		int len = block.Length;
		var result = new bool[len];

		for (var index = 0; index < len ; index++)
		{
			result[(index + amount) % len] = block[index];
		}
		return result;
	}
	
	public void RotateRow(int rowindex, int amount)
	{
		var row = GetRow(rowindex);
		var newrow = RotateBlock(row, amount);
		SetRow(rowindex, newrow);
	}

    public void RotateCol(int colindex, int amount)
    {
        var col = GetCol(colindex);
        var newcol = RotateBlock(col, amount);
        SetCol(colindex, newcol);
    }

    public int HowManyLit
	{
		get
		{
			int result = 0;
			foreach (var cell in cells)
			{
				if (cell)
					result++;
			}
			return result;
		}
	}
	
	public void DrawGrid()
	{
		for (var rowindex = 0; rowindex < _height; rowindex++)
		{
			for (var colindex = 0; colindex < _width; colindex++)
			{
				Console.Write(cells[(rowindex * _width) + colindex] == true ? "1" : "0");
            }
			Console.WriteLine();
        }
	}
}

public IEnumerable<string> GetInput()
{
#if TEST
	var result = new List<string>
	{
		"rect 3x2",
		"rotate column x=1 by 1",
		"rotate row y=0 by 4",
		"rotate column x=1 by 1"
	};
#else
	string fname = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), "day8-input.txt");
	var result = File.ReadAllLines(fname);
#endif
	return result;
}