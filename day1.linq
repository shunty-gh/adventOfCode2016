<Query Kind="Program" />

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
}

public int GetFinalPosition(string input)
{
	var directions = input.Split(',').Select(x => x.Trim()).ToList();
	//directions.Dump();

	var current = new Position();
	foreach (var direction in directions)
	{
		current.Move(direction);
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

public enum Facing
{
	North,
	East,
	South,
	West,
}

public static class FacingExtensions
{
	public static Facing Turn(this Facing current, string direction)
	{
		return (direction.ToUpper() == "L") ? current.TurnLeft() : current.TurnRight();		
	}
	
	public static Facing TurnLeft(this Facing current)
	{
		switch (current)
		{
			case Facing.North:
				return Facing.West;
			case Facing.East:
				return Facing.North;
			case Facing.South:
				return Facing.East;
			case Facing.West:
				return Facing.South;
			default:
				return Facing.North;
		}
	}

	public static Facing TurnRight(this Facing current)
	{
		switch (current)
		{
			case Facing.North:
				return Facing.East;
			case Facing.East:
				return Facing.South;
			case Facing.South:
				return Facing.West;
			case Facing.West:
				return Facing.North;
			default:
				return Facing.North;
		}
	}
}

public class Position
{
	private int x = 0;
	private int y = 0;
	private Facing facing = Facing.North;

	public void Move(string instruction)
	{
		var dir = instruction[0];
		var goleft = dir == 'L';
		var distance = int.Parse(instruction.Substring(1));

		switch (facing)
		{
			case Facing.North:
				x += distance * (goleft ? -1 : 1);
				break;
			case Facing.East:
				y += distance * (goleft ? 1 : -1);
				break;
			case Facing.South:
				x += distance * (goleft ? 1 : -1);
				break;
			case Facing.West:
				y += distance * (goleft ? -1 : 1);
				break;
			default:
				break;
		}
		facing = facing.Turn(dir.ToString());
		$"Moved {instruction}. New location x={x}; y={y}".Dump();
	}

	public int DistanceFromOrigin
	{
		get
		{
			return Math.Abs(x) + Math.Abs(y);
		}
	}
}
