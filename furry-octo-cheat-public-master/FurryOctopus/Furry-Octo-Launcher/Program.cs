using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace Furry_Octo_Launcher
{
    class Program
    {
        private static List<string> Arguments;
        public static string ServerAddress;
        private static int Port;

        private static TcpClient client;
        static void Main(string[] args)
        {

            ServerAddress = "owenbones.co.uk";
            Port = 3001;
            Console.WriteLine("TURDHACK LAUNCHED");
            Console.WriteLine("Enter username and password");
            string user, pass;

            Console.Write("Username: ");
            user = Console.ReadLine();
            while (string.IsNullOrEmpty(user) || string.IsNullOrWhiteSpace(user) || user.Length <= 3)
            {
                Console.Clear();
                Console.WriteLine("TURDHACK LAUNCHED");
                Console.WriteLine("Enter username and password");
                Console.Write("Username: ");
                user = Console.ReadLine();
            }

            Console.Write("Password: ");
            pass = Console.ReadLine();

            while (string.IsNullOrEmpty(pass) || string.IsNullOrWhiteSpace(pass) || pass.Length <= 3)
            {
                Console.Clear();
                Console.WriteLine("TURDHACK LAUNCHED");
                Console.WriteLine("Enter username and password");
                Console.WriteLine("Username: {0}", user);
                Console.Write("Password: ");
                pass = Console.ReadLine();
            }

            try
            {
                client = new TcpClient();
                client.ReceiveTimeout = 1000;
                client.Connect(ServerAddress, Port);
            }
            catch
            {
                Console.WriteLine("Failed to connect");
                goto Terminate;
            }
            StreamWriter sw = new StreamWriter(client.GetStream());
            StreamReader sr = new StreamReader(client.GetStream());


            sw.WriteLine(user + ":" + pass);
            sw.Flush();

            user = "null";
            pass = "null";

            string reply = "";
            try
            {
                reply += (sr.ReadToEnd().Trim());
                string[] shitreply = reply.Split(new char[] { '\n', ':' });
                Console.WriteLine(reply + "\n");
                int c = 0;
                foreach (string s in shitreply)
                {
                    Console.WriteLine("{0}: {1}", c, s);
                    c++;
                }

                string uid = shitreply[2];
                string ent = shitreply[3];
                string lply = shitreply[4];
                string cid = shitreply[5];

                string[] c_args = new string[5];
                c_args[0] = (new Random().Next(10, 99)).ToString();
                c_args[1] = uid;
                c_args[2] = ent;
                c_args[3] = lply;
                c_args[4] = cid;
                System.Diagnostics.Process.Start("Furry-Octo-App.exe", String.Join(" ", c_args)); // test test
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                Console.WriteLine("No response from login server");
                Console.ReadLine();
                goto Close;
            }


        Close:
            sw.Close();
            sw.Dispose();
            sr.Close();
            sr.Dispose();
        Terminate:
            client.Close();
            return;
        }
    }
}
