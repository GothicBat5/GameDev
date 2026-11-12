using System;

namespace OneCompiler
{
	public class LevelP
	{
		public static void Main(string[] args)
		{
			int level = 1; 
			int IHP = 10;

			while(20 >= level) 
			{
				int hp = ((level - 1) * IHP) + 100;

				Console.WriteLine($"Level {level}. \nHP: {hp} \n");

				level++; 
			}
		}
	}
}
