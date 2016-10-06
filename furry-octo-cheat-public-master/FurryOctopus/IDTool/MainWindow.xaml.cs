using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;

namespace BuildTool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        const string FILE_NAME = "app.cpp";
        const string VARIABLE_NAME = "ProgramID";

        string[] lines;
        int lineCount = 0;

        int varLineNumber = 0;
        string[] varSplitLine;

        Random rand;

        public MainWindow()
        {
            InitializeComponent();

            rand = new Random((int)System.DateTime.Today.TimeOfDay.Ticks);

            bool varFound = false;

            if (!File.Exists(FILE_NAME))
            {
                ShowErrorBox("Couldn't find " + FILE_NAME + ".");
                Exit();
            }

            using (StreamReader sR = new StreamReader(FILE_NAME))
            {

                // Count lines
                while (true)
                {
                    char c = (char)sR.Read();
                    if (c == '\n')
                    {
                        lineCount++;
                    }
                    else if (c == (char)65535)
                    {
                        break;
                    }
                }

                // Make suitably sized string array
                lines = new string[lineCount];

                // Reset stream
                sR.BaseStream.Position = 0;
                sR.DiscardBufferedData();

                // Read lines into string array
                for (int i = 0; i < lineCount; i++)
                {
                    lines[i] = sR.ReadLine();

                    if (lines[i].Contains("string " + VARIABLE_NAME))
                    {
                        varLineNumber = i;
                        varSplitLine = lines[i].Split('\"');
                        varFound = true;
                    }
                }

                // Display error and close if the variable wasn't found
                if (!varFound)
                {
                    ShowErrorBox("Variable " + VARIABLE_NAME + " not found.");
                    Exit();
                }
            }

            idBox.Text = varSplitLine[1];
        }

        private void generateButton_Click(object sender, RoutedEventArgs e)
        {
            string randID = rand.Next(0, 10000000).ToString();
            int charsShort = 7 - randID.Length;
            for (int i = 0; i < charsShort; i++)
            {
                randID += rand.Next(0, 10);
            }

            idBox.Text = randID;
        }

        private void applyButton_Click(object sender, RoutedEventArgs e)
        {
            varSplitLine[1] = idBox.Text;
            lines[varLineNumber] = varSplitLine[0] + '\"' + varSplitLine[1] + '\"' + varSplitLine[2];

            using (StreamWriter sW = new StreamWriter(FILE_NAME))
            {
                for (int i = 0; i < lineCount; i++)
                {
                    sW.WriteLine(lines[i]);
                }
            }

            Exit();
        }

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            Exit();
        }

        private void ShowErrorBox(string message)
        {
            MessageBox.Show(
                    message,
                    string.Empty, MessageBoxButton.OK,
                    MessageBoxImage.Error);
        }

        private void Exit()
        {
            Environment.Exit(0);
        }

        
    }
}
