<Query Kind="Program" />

//#define TEST

void Main()
{
	// Advent of code 2016
	// Day 10
	// http://adventofcode.com/2016/day/10
    var input = GetInput();
	var result = GetResult(input);
    result = (outputs[0].First() * outputs[1].First() * outputs[2].First());
    BigBot.Dump("Bot Id");
    result.Dump("Result 1");
}

public static string init_pattern = @"value (?<num>\d*) goes to bot (?<botid>\d*)";
Regex re_init = new Regex(init_pattern);

public static string inst_pattern = @"bot (?<botid>\d*) gives low to (?<low>(bot|output)) (?<loid>\d*) and high to (?<high>(bot|output)) (?<hiid>\d*)";
Regex re_inst = new Regex(inst_pattern);

public Dictionary<int, BotInstruction> BotInstructions = new Dictionary<int, BotInstruction>();

public Int64 GetResult(IEnumerable<string> input)
{
    Func<string, Tuple<int, int>> ParseInitInstruction = (s) =>
    {
        var match = re_init.Match(s);
        return new Tuple<int, int>(int.Parse(match.Groups["botid"].Value), int.Parse(match.Groups["num"].Value));
    };

    Func<string, BotInstruction> ParseBotInstruction = (s) =>
    {
        var match = re_inst.Match(s);
        return new BotInstruction
        {
            FromBotId = int.Parse(match.Groups["botid"].Value),
            LoIsBot = match.Groups["low"].Value == "bot",
            HiIsBot = match.Groups["high"].Value == "bot",
            LoId = int.Parse(match.Groups["loid"].Value),
            HiId = int.Parse(match.Groups["hiid"].Value),
        };
    };

    Int64 result = 0;
    // Split input into "value... to..." and "bot... gives..."
    var inits = input.Where(l => l.StartsWith("value"));
    var instructions = input.Where(l => l.StartsWith("bot"));

    // Load instructions
    foreach (var line in instructions)
    {
        var inst = ParseBotInstruction(line);
        BotInstructions.Add(inst.FromBotId, inst);
    }

    // Initialize bots
    foreach (var line in inits)
    {
        var inst = ParseInitInstruction(line);
        InitializeBot(inst);
    }

    //bots.Dump();
    //outputs.Dump();

    return result;
}

public int BigBot = 0; // A lovely global to handle the id of the bot that compares the required chips
public Dictionary<int, Bot> bots = new Dictionary<int, Bot>();
public Dictionary<int, List<int>> outputs = new Dictionary<int, List<int>>();

public Bot InitializeBot(Tuple<int, int> instruction)
{
    Bot bot = GetOrCreateBot(instruction.Item1);
    bot.Receive(instruction.Item2);
    return bot;
}

public Bot GetOrCreateBot(int id)
{
    if (bots.ContainsKey(id))
        return bots[id];
    else
    {
        var bot = new Bot(id);
        bot.OnReceivedChip += (sender, args) =>
        {
            Console.WriteLine($"Bot {args.BotId} received chip {args.Chip}");
            var b = (Bot)sender;
#if TEST            
            if ((b.Low == 2) && (b.High == 5))
            {
                Console.WriteLine($"Bot {args.BotId} compares value-2 and value-5 chips");
            }
#else
            if ((b.Low == 17) && (b.High == 61))
            {
                BigBot = args.BotId;
                Console.WriteLine($"Bot {args.BotId} compares value-17 and value-61 chips");
            }
#endif       
        };
        bot.OnHasTwoChips += (sender, args) =>
        {
            // NB!! Totally NOT thread-safe - but doesn't matter 'cos only 1 instruction per bot and instruction list never gets added to
            
            // Find an instruction for this bot and apply it
            if (BotInstructions.ContainsKey(args.BotId))
            {
                var instruction = BotInstructions[args.BotId];
                ApplyBotInstruction(instruction);
                
                // Remove it
                BotInstructions.Remove(args.BotId);
            }
        };
        bots.Add(id, bot);
        return bot;
    }
}

public List<int> GetOrCreateOutput(int id)
{
    if (outputs.ContainsKey(id))
        return outputs[id];
    else
    {
        var output = new List<int>();
        outputs.Add(id, output);
        return output;
    }
}

public void ApplyBotInstruction(BotInstruction instruction)
{
    Bot bot = GetOrCreateBot(instruction.FromBotId);

    int lo = bot.GiveLow();
    int hi = bot.GiveHigh();
    
    int lid = instruction.LoId;
    if (instruction.LoIsBot)
    {
        var tobot = GetOrCreateBot(lid);
        tobot.Receive(lo);
    }
    else
    {
        var output = GetOrCreateOutput(lid);
        output.Add(lo);
    }

    int hid = instruction.HiId;
    if (instruction.HiIsBot)
    {
        var tobot = GetOrCreateBot(hid);
        tobot.Receive(hi);
    }
    else
    {
        var output = GetOrCreateOutput(hid);
        output.Add(hi);
    }
}

public class BotInstruction
{
    public int FromBotId { get; set; } = -1;
    public bool LoIsBot { get; set; } = false;
    public bool HiIsBot { get; set; } = false;
    public int LoId { get; set; } = 0;
    public int HiId { get; set; } = 0;
}

public class ReceivedChipEventArgs : EventArgs
{
    public int BotId { get; set; }
    public int Chip { get; set; }
}

public class HasTwoChipEventArgs : EventArgs
{
    public int BotId { get; set; }
}

public class Bot
{
    private int _id;
    private int _chip1 = 0;
    private int _chip2 = 0;
    public Bot(int id, int chip = 0)
    {
        _id = id;
        Receive(chip);
    }
    public int Id { get { return _id; } }
    public int Low { get { return _chip1 < _chip2 ? _chip1 : _chip2; } }
    public int High { get { return _chip1 > _chip2 ? _chip1 : _chip2; } }
    public bool HasTwoChips { get { return (_chip1 > 0) && (_chip2 > 0); } }

    public event EventHandler<ReceivedChipEventArgs> OnReceivedChip;
    public event EventHandler<HasTwoChipEventArgs> OnHasTwoChips;

    private void ReceivedChip(int chip)
    {
        OnReceivedChip?.Invoke(this, new ReceivedChipEventArgs { BotId = _id, Chip = chip });

        if (this.HasTwoChips)
            OnHasTwoChips?.Invoke(this, new HasTwoChipEventArgs { BotId = _id });
    }
    
    public void Receive(int chip)
    {
        if (_chip1 <= 0)
            _chip1 = chip;
        else
            _chip2 = chip;
        
        ReceivedChip(chip);
    }
    
    public int GiveLow()
    {
        int result;
        if (_chip1 < _chip2)
        {
            result = _chip1;
            _chip1 = 0;
        }
        else
        {
            result = _chip2;
            _chip2 = 0;
        }
        return result;
    }

    public int GiveHigh()
    {
        int result;
        if (_chip1 > _chip2)
        {
            result = _chip1;
            _chip1 = 0;
        }
        else
        {
            result = _chip2;
            _chip2 = 0;
        }
        return result;
    }
}

public IEnumerable<string> GetInput()
{
#if TEST
	var result = new List<string>
	{
        "value 5 goes to bot 2",
        "bot 2 gives low to bot 1 and high to bot 0",
        "value 3 goes to bot 1",
        "bot 1 gives low to output 1 and high to bot 0",
        "bot 0 gives low to output 2 and high to output 0",
        "value 2 goes to bot 2",
    };
#else 
	string fname = Path.Combine(
        Path.GetDirectoryName(Util.CurrentQueryPath), 
        Path.GetFileNameWithoutExtension(Util.CurrentQueryPath) + "-input.txt");
        
	var result = File.ReadAllLines(fname);
#endif
	return result;
}