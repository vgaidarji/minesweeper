using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Minesweeper
{
    /// <summary>
    /// Game difficulty which has influence over game objects.
    /// 
    /// mines count: easy - 10, normal - 40, hard - 99
    /// fieldCells (rows*columns): easy - 81(9*9), normal - 256(16*16), hard - 496 (16*31) 
    /// 
    /// </summary>
    internal class GameDifficulty : GameDifficultyEnumeration
    {
        public static GameDifficulty Easy = new GameDifficulty(9, 9, 10, 204, 295, 167, 167);
        public static GameDifficulty Normal = new GameDifficulty(16, 16, 40, 331, 423, 294, 294);
        public static GameDifficulty Hard = new GameDifficulty(31, 16, 99, 602, 423, 564, 294);

        public GameDifficulty(int rows, int columns, int numberOfMines, int formWidth, int formHeight, int fieldWidth, int fieldHeight)
            : base(rows, columns, numberOfMines, formWidth, formHeight, fieldWidth, fieldHeight) { }
    }

    internal abstract class GameDifficultyEnumeration : IComparable
    {
        public int Rows { get; private set; }

        public int Columns { get; private set; }

        public int NumberOfMines { get; private set; }

        public int FormWidth { get; private set; }

        public int FormHeight { get; private set; }

        public int FieldWidth { get; private set; }

        public int FieldHeight { get; private set; }

        protected GameDifficultyEnumeration(int rows, int columns, int numberOfMines, int formWidth, int formHeight, int fieldWidth, int fieldHeight) => 
            (Rows, Columns, NumberOfMines, FormWidth, FormHeight, FieldWidth, FieldHeight) = (rows, columns, numberOfMines, formWidth, formHeight, fieldWidth, fieldHeight);

        public override string ToString() => NumberOfMines.ToString();

        public static IEnumerable<T> GetAll<T>() where T : GameDifficultyEnumeration =>
            typeof(T).GetFields(BindingFlags.Public |
                                BindingFlags.Static |
                                BindingFlags.DeclaredOnly)
                     .Select(f => f.GetValue(null))
                     .Cast<T>();

        public int CompareTo(object other) => NumberOfMines.CompareTo(((GameDifficultyEnumeration)other).NumberOfMines);

        public override bool Equals(object obj)
        {
            return obj is GameDifficultyEnumeration enumeration &&
                   Rows == enumeration.Rows &&
                   Columns == enumeration.Columns &&
                   NumberOfMines == enumeration.NumberOfMines &&
                   FormWidth == enumeration.FormWidth &&
                   FormHeight == enumeration.FormHeight &&
                   FieldWidth == enumeration.FieldWidth &&
                   FieldHeight == enumeration.FieldHeight;
        }

        public override int GetHashCode()
        {
            int hashCode = 1060049121;
            hashCode = hashCode * -1521134295 + Rows.GetHashCode();
            hashCode = hashCode * -1521134295 + Columns.GetHashCode();
            hashCode = hashCode * -1521134295 + NumberOfMines.GetHashCode();
            hashCode = hashCode * -1521134295 + FormWidth.GetHashCode();
            hashCode = hashCode * -1521134295 + FormHeight.GetHashCode();
            hashCode = hashCode * -1521134295 + FieldWidth.GetHashCode();
            hashCode = hashCode * -1521134295 + FieldHeight.GetHashCode();
            return hashCode;
        }
    }
}
