using System;
using System.Collections.Generic;
using System.IO;

namespace Day_4
{
    class Program
    {
        enum TaskType
        {
            ChangeGuard = 0,
            Asleep = 1,
            WakeUp = 2
        }

        struct GuardTask
        {
            public DateTime date;
            public TaskType task;
            public int guardId;
        }

        struct MinuteTimes
        {
            public int[] minutes;
            public int maxValue;
        }

        static List<GuardTask> guardTasks = new List<GuardTask>();

        static Dictionary<int, int> guardSleepMinutes = new Dictionary<int, int>();
        static Dictionary<int, MinuteTimes> minutesSleptGuards = new Dictionary<int, MinuteTimes>();

        static int[] minuteCounts = new int[60];

        static int Atoi(ref string line, ref int index)
        {
            int result = 0;

            while(index < line.Length)
            {
                if(line[index] < '0' || line[index] > '9')
                {
                    return result;
                }

                result *= 10;
                result += (line[index] - '0');

                index++;
            }

            return result;
        }

        static void ParseLog(string log)
        {
            int index = 0;

            while(index < log.Length)
            {
                if(log[index] == '[')
                {
                    GuardTask task = new GuardTask();
                    int year, month, day, hour, minute;

                    TaskType taskType = TaskType.ChangeGuard;
                    int guardId = -1;

                    index++;
                    year = Atoi(ref log, ref index);
                    index++;
                    month = Atoi(ref log, ref index);
                    index++;
                    day = Atoi(ref log, ref index);
                    index++;
                    hour = Atoi(ref log, ref index);
                    index++;
                    minute = Atoi(ref log, ref index);

                    index++;
                    index++;

                    switch(log[index])
                    {
                        case 'G':
                            index += 7;
                            guardId = Atoi(ref log, ref index);
                            break;
                        case 'f':
                            taskType = TaskType.Asleep;
                            break;
                        case 'w':
                            taskType = TaskType.WakeUp;
                            break;
                        default:
                            Console.WriteLine("Invalid Input data");
                            break;
                    }

                    
                    task.date = new DateTime(year, month, day, hour, minute, 0);
                    task.task = taskType;
                    task.guardId = guardId;

                    guardTasks.Add(task);
                }

                index++;
            }
        }

        static void DebugWriteList()
        {
            foreach(var task in guardTasks)
            {
                Console.WriteLine("{0} {1} {2} {3} {4} {5} {6}", task.date.Year, task.date.Month, task.date.Day, task.date.Hour, task.date.Minute, task.guardId, task.task);
            }
        }
        
        static void SortList()
        {
            guardTasks.Sort((task1, task2) => 
            {
                return task1.date.CompareTo(task2.date);
            });
        }

        static int SolveFirst()
        {
            int guardId = -1;
            int lazyGuard;
            int maxMinute = 0;
            DateTime sleepDate = new DateTime();

            for(int i = 0; i < 60; i++)
            {
                minuteCounts[i] = 0;
            }

            foreach(var task in guardTasks)
            {
                if(task.task == TaskType.ChangeGuard)
                {
                    guardId = task.guardId;
                }

                if(task.task == TaskType.Asleep)
                {
                    sleepDate = task.date;
                }

                if(task.task == TaskType.WakeUp)
                {
                    TimeSpan deltaTime = task.date - sleepDate;

                    if (!guardSleepMinutes.ContainsKey(guardId))
                    {
                        guardSleepMinutes[guardId] = (int)deltaTime.TotalMinutes;
                    }
                    else
                    {
                        guardSleepMinutes[guardId] += (int)deltaTime.TotalMinutes;
                    }
                }
            }

            lazyGuard = guardId;

            foreach(var guard in guardSleepMinutes)
            {
                if(guard.Value > guardSleepMinutes[lazyGuard])
                {
                    lazyGuard = guard.Key;
                }
            }

            foreach (var task in guardTasks)
            {
                if (task.task == TaskType.ChangeGuard)
                {
                    guardId = task.guardId;
                }

                if (guardId == lazyGuard)
                {
                    if (task.task == TaskType.Asleep)
                    {
                        sleepDate = task.date;
                    }

                    if (task.task == TaskType.WakeUp)
                    {
                        while(sleepDate < task.date)
                        {
                            minuteCounts[sleepDate.Minute]++;

                            if(minuteCounts[sleepDate.Minute] > minuteCounts[maxMinute])
                            {
                                maxMinute = sleepDate.Minute;
                            }

                            sleepDate = sleepDate.AddMinutes(1);
                        }
                    }
                }
            }
            
            return lazyGuard * maxMinute;
        }

        static int SolveSecond()
        {
            int guardId = -1;
            DateTime sleepDate = new DateTime();
            int maxGuardSleep;
            int maxMinuteSleep = 0;

            foreach (var task in guardTasks)
            {
                if (task.task == TaskType.ChangeGuard)
                {
                    guardId = task.guardId;
                }
                
                if (task.task == TaskType.Asleep)
                {
                    sleepDate = task.date;
                }

                if (task.task == TaskType.WakeUp)
                {
                    while (sleepDate < task.date)
                    {
                        if(!minutesSleptGuards.ContainsKey(guardId))
                        {
                            MinuteTimes minuteTimes = new MinuteTimes();

                            minuteTimes.minutes = new int[60];
                            minuteTimes.maxValue = 0;

                            for(int i = 0; i < 60; i++)
                            {
                                minuteTimes.minutes[i] = 0;
                            }

                            minuteTimes.minutes[sleepDate.Minute]++;

                            if(minuteTimes.minutes[sleepDate.Minute] > minuteTimes.maxValue)
                            {
                                minuteTimes.maxValue = minuteTimes.minutes[sleepDate.Minute];
                            }

                            minutesSleptGuards[guardId] = minuteTimes;
                        }
                        else
                        {
                            minutesSleptGuards[guardId].minutes[sleepDate.Minute]++;
                            if(minutesSleptGuards[guardId].minutes[sleepDate.Minute] > minutesSleptGuards[guardId].maxValue)
                            {
                                MinuteTimes minuteTimes = minutesSleptGuards[guardId];
                                minuteTimes.maxValue = minutesSleptGuards[guardId].minutes[sleepDate.Minute];
                                minutesSleptGuards[guardId] = minuteTimes;
                            }
                        }
                        sleepDate = sleepDate.AddMinutes(1);
                    }
                }
            }

            maxGuardSleep = guardId;

            foreach(var guard in minutesSleptGuards)
            {
                if(minutesSleptGuards[maxGuardSleep].maxValue < guard.Value.maxValue)
                {
                    maxGuardSleep = guard.Key;
                }
            }

            for(int i = 0; i < 60; i++)
            {
                if(minutesSleptGuards[maxGuardSleep].minutes[i] > minutesSleptGuards[maxGuardSleep].minutes[maxMinuteSleep])
                {
                    maxMinuteSleep = i;
                }
            }

            return maxGuardSleep * maxMinuteSleep;
        }

        static void Main(string[] args)
        {
            string log = File.ReadAllText("log.in");

            ParseLog(log);
            SortList();
            
            int result1 = SolveFirst();
            int result2 = SolveSecond();

            File.WriteAllText("log.out", String.Format("{0}\r\n{1}", result1, result2));
        }
    }
}
