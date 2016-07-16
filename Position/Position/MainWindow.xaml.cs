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
using System.IO.Ports;

namespace Position
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string[] ports;//可用串口数组

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.SystemKey == Key.LeftAlt || e.SystemKey == Key.RightAlt)
            {
                if (Visibility.Collapsed == Postset.Visibility)
                {
                    Postset.Visibility = Visibility.Visible;
                }
                else if (Visibility.Visible == Postset.Visibility)
                {
                    Postset.Visibility = Visibility.Collapsed;
                }

            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)//主窗口初始化
        {
            ports = SerialPort.GetPortNames();//获取可用串口            

            if (ports.Length > 0)//ports.Length > 0说明有串口可用
            {
                for (int i = 0; i < ports.Length; i++)
                {
                    ComboBoxItem item = new ComboBoxItem();
                    item.Content = ports[i];
                    Portcobox.Items.Add(item);//下拉控件里添加可用串口
                }
                Portcobox.SelectedIndex = 0;
            }
        }
    }
}
