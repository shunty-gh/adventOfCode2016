<Query Kind="Program">
  <Namespace>System.Drawing</Namespace>
</Query>

void Main()
{
	// Advent of code 2016
	// Day 1
	// http://adventofcode.com/2016/day/1
	
	var input = GetTestInput(1);
	var result = GetFinalPosition(input);
	result.Dump("Test input 1");

	input = GetTestInput(2);
	result = GetFinalPosition(input);
	result.Dump("Test input 2");

	input = GetTestInput(3);
	result = GetFinalPosition(input);
	result.Dump("Test input 3");

	input = GetInput();
	result = GetFinalPosition(input);
	result.Dump("Main");

    result = GetFinalPosition(input, true);
    result.Dump("Main 2");
}

public int GetFinalPosition(string input, bool stopAfterSecondVisit = false)
{
	var directions = input.Split(',').Select(x => x.Trim()).ToList();
	//directions.Dump();

	var current = new Position();
	foreach (var direction in directions)
	{
		current.Move(direction, stopAfterSecondVisit);
        if (current.FoundIt)
            break;
	}
	return current.DistanceFromOrigin;
}

public string GetTestInput(int testNumber)
{
	switch (testNumber)
	{
		case 1:
			return "R2, L3";
		case 2:
			return "R2, R2, R2";
		case 3:
			return "R5, L5, R5, R3";
		default:
			return "";
	}
}

public string GetInput()
{
	return "R3, R1, R4, L4, R3, R1, R1, L3, L5, L5, L3, R1, R4, L2, L1, R3, L3, R2, R1, R1, L5, L2, L1, R2, L4, R1, L2, L4, R2, R2, L2, L4, L3, R1, R4, R3, L1, R1, L5, R4, L2, R185, L2, R4, R49, L3, L4, R5, R1, R1, L1, L1, R2, L1, L4, R4, R5, R4, L3, L5, R1, R71, L1, R1, R186, L5, L2, R5, R4, R1, L5, L2, R3, R2, R5, R5, R4, R1, R4, R2, L1, R4, L1, L4, L5, L4, R4, R5, R1, L2, L4, L1, L5, L3, L5, R2, L5, R4, L4, R3, R3, R1, R4, L1, L2, R2, L1, R4, R2, R2, R5, R2, R5, L1, R1, L4, R5, R4, R2, R4, L5, R3, R2, R5, R3, L3, L5, L4, L3, L2, L2, R3, R2, L1, L1, L5, R1, L3, R3, R4, R5, L3, L5, R1, L3, L5, L5, L2, R1, L3, L1, L3, R4, L1, R3, L2, L2, R3, R3, R4, R4, R1, L4, R1, L5";
}

public enum CompassDirection
{
	North,
	East,
	South,
	West,
}

public static class FacingExtensions
{
	public static CompassDirection Turn(this CompassDirection current, string direction)
	{
		return (direction.ToUpper() == "L") ? current.TurnLeft() : current.TurnRight();		
	}
	
	public static CompassDirection TurnLeft(this CompassDirection current)
	{
		switch (current)
		{
			case CompassDirection.North:
				return CompassDirection.West;
			case CompassDirection.East:
				return CompassDirection.North;
			case CompassDirection.South:
				return CompassDirection.East;
			case CompassDirection.West:
				return CompassDirection.South;
			default:
				return CompassDirection.North;
		}
	}

	public static CompassDirection TurnRight(this CompassDirection current)
	{
		switch (current)
		{
			case CompassDirection.North:
				return CompassDirection.East;
			case CompassDirection.East:
				return CompassDirection.South;
			case CompassDirection.South:
				return CompassDirection.West;
			case CompassDirection.West:
				return CompassDirection.North;
			default:
				return CompassDirection.North;
		}
	}
}

public class Position
{
	private int x = 0;
	private int y = 0;
    private bool foundIt = false;
	private CompassDirection facing = CompassDirection.North;
    private List<Point> visited = new List<Point>();

	public void Move(string instruction, bool stopAfterSecondVisit)
	{
        if (stopAfterSecondVisit && foundIt)
            return;
            
		var dir = instruction[0].ToString();
        var newdirection = facing.Turn(dir);
		var distance = int.Parse(instruction.Substring(1));

        for (int step = 1; step <= distance; step++)
        {
            x += (newdirection == CompassDirection.North) ? 1 : (newdirection == CompassDirection.South) ? -1 : 0;
            y += (newdirection == CompassDirection.East) ? 1 : (newdirection == CompassDirection.West) ? -1 : 0;

            if (stopAfterSecondVisit)
            {
                Point here = new Point(x, y);
                if (visited.IndexOf(here) >= 0)
                {
                    // We're here
                    foundIt = true;
                    break;
                }
                visited.Add(here);
            }
        }
        
		facing = newdirection;
		$"Moved {instruction}. New location x={x}; y={y}".Dump();
	}

    public bool FoundIt { get { return foundIt; } }
    
	public int DistanceFromOrigin
	{
		get
		{
			return Math.Abs(x) + Math.Abs(y);
		}
	}
}
