using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Day6
{
    public partial class Form1 : Form
    {
        private const int MATRIX_WIDTH = 2000;
        private const int MATRIX_HEIGHT = 2000;

        public struct CoordType
        {
            public int id;
            public int x;
            public int y;
        }

        struct MatrixCellType
        {
            public int id;
            public int iteration;
        }

        struct QueueType
        {
            public int x;
            public int y;
        }

        private List<CoordType> m_coords = new List<CoordType>();
        private Queue<QueueType> m_leeQueue = new Queue<QueueType>();
        private MatrixCellType[,] m_coordMatrix = new MatrixCellType[MATRIX_HEIGHT, MATRIX_WIDTH];

        private bool[] infinite;
        private int[] areas;

        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private int Atoi(ref string line, ref int index)
        {
            int result = 0;

            while(index < line.Length)
            {
                if (line[index] < '0' || line[index] > '9')
                    return result;

                result = (result * 10) + (line[index] - '0');

                index++;
            }

            return result;
        }

        private void ParseInput(ref string input)
        {
            int index = 0;
            int id = 0;

            while (index < input.Length)
            {
                if(input[index] >= '0' && input[index] <= '9')
                {
                    int coordX = Atoi(ref input, ref index);
                    index += 2;
                    int coordY = Atoi(ref input, ref index);

                    CoordType newCoord = new CoordType();

                    newCoord.x = coordX;
                    newCoord.y = coordY;
                    newCoord.id = id;

                    m_coords.Add(newCoord);

                    id++;
                }

                index++;
            }

            infinite = new bool[id];
            areas = new int[id];

            for(int i = 0; i < id; i++)
            {
                infinite[i] = false;
                areas[i] = 0;
            }
        }

        private void DebugInputData()
        {
            foreach(var coord in m_coords)
            {
                Debug.WriteLine("{0} {1} {2}", coord.id, coord.x, coord.y);
            }
        }

        void DebugMatrix()
        {
            for(int i = 0; i < 400; i++)
            {
                for(int j = 0; j < 400; j++)
                {
                    Debug.Write(m_coordMatrix[i, j].id);
                    Debug.Write(" ");
                }
                Debug.WriteLine("");
            }
        }
        
        bool ValidCoords(int y, int x)
        {
            if(y >= 0 && y < MATRIX_HEIGHT &&
               x >= 0 && x < MATRIX_WIDTH)
            {
                return true;
            }

            return false;
        }

        void Lee()
        {
            int[] dirY = { 0, -1, 0, 1 };
            int[] dirX = { -1, 0, 1, 0 };

            while(m_leeQueue.Count > 0)
            {
                QueueType queueType = m_leeQueue.Dequeue();

                int id = m_coordMatrix[queueType.y, queueType.x].id;
                if (id != -1)
                {
                    int iteration = m_coordMatrix[queueType.y, queueType.x].iteration;

                    for (int i = 0; i < 4; i++)
                    {
                        int newY = dirY[i] + queueType.y;
                        int newX = dirX[i] + queueType.x;

                        if (ValidCoords(newY, newX))
                        {
                            if (m_coordMatrix[newY, newX].iteration == -1)
                            {
                                m_coordMatrix[newY, newX].iteration = iteration + 1;
                                m_coordMatrix[newY, newX].id = id;

                                QueueType newQueueElement = new QueueType();

                                newQueueElement.y = newY;
                                newQueueElement.x = newX;

                                m_leeQueue.Enqueue(newQueueElement);
                            }

                            if (m_coordMatrix[newY, newX].iteration == iteration + 1)
                            {
                                if (m_coordMatrix[newY, newX].id != id)
                                {
                                    m_coordMatrix[newY, newX].id = -1;
                                }
                            }
                        }
                        else
                        {
                            infinite[id] = true;
                        }
                    }
                }
            }
        }

        int CountMax()
        {
            int maxArea = 0;

            for(int i = 0; i < MATRIX_HEIGHT; i++)
            {
                for(int j = 0; j < MATRIX_WIDTH; j++)
                {
                    if (m_coordMatrix[i, j].id != -1)
                    {
                        if (!infinite[m_coordMatrix[i, j].id])
                        {
                            areas[m_coordMatrix[i, j].id]++;
                        }
                    }
                }
            }

            for(int i = 0; i < areas.Length; i++)
            {
                if (areas[i] > maxArea)
                    maxArea = areas[i];
            }

            return maxArea;
        }

        int Solve1()
        {
            for(int i = 0; i < MATRIX_HEIGHT; i++)
            {
                for(int j = 0; j < MATRIX_WIDTH; j++)
                {
                    MatrixCellType cell = new MatrixCellType();

                    cell.id = -1;
                    cell.iteration = -1;

                    m_coordMatrix[i,j] = cell;
                }
            }

            foreach (var coord in m_coords)
            {
                QueueType queueType = new QueueType();

                queueType.x = coord.x;
                queueType.y = coord.y;

                m_leeQueue.Enqueue(queueType);

                m_coordMatrix[coord.y, coord.x].id = coord.id;
                m_coordMatrix[coord.y, coord.x].iteration = 0;
            }

            Lee();

            return CountMax();
        }

        int Distance(int y1, int x1, int y2, int x2)
        {
            int yDiff = Math.Abs(y2 - y1);
            int xDiff = Math.Abs(x2 - x1);

            return yDiff + xDiff;
        }

        int Solve2()
        {
            int result = 0;

            for(int y = 0; y < MATRIX_HEIGHT; y++)
            {
                for(int x = 0; x < MATRIX_WIDTH; x++)
                {
                    int distance = 0;

                    foreach(var coord in m_coords)
                    {
                        distance += Distance(y, x, coord.y, coord.x);
                    }

                    if(distance < 10000)
                    {
                        result++;
                    }
                }
            }

            return result;
        }

        private void Solve()
        {
            string text = textBox1.Text;
            ParseInput(ref text);
            
            int result1 = Solve1();
            int result2 = Solve2();

            label4.Text = result1.ToString();
            label3.Text = result2.ToString();

            m_coordMatrix = new MatrixCellType[MATRIX_HEIGHT, MATRIX_WIDTH];
            m_coords = new List<CoordType>();
            m_leeQueue = new Queue<QueueType>();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Solve();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }
    }
}
