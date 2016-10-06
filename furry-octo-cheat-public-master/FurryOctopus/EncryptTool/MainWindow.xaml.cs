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

namespace EncryptTool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void stringBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            string key = keyBox.Text;
            string msg = stringBox.Text;

            while (msg.Length > key.Length)
            {
                key += keyBox.Text;
            }

            string cipher = string.Empty;

            for (int i = 0; i < stringBox.Text.Length; i++)
            {
                cipher += '_';
                cipher += msg[i] ^ key[i];
            }

            cipherBox.Text = cipher;
        }

        private void copyButton_Click(object sender, RoutedEventArgs e)
        {
            Clipboard.SetText(cipherBox.Text);
        }
    }
}
