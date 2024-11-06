namespace Minesweeper
{
    public partial class App : Application
    {
        private const int APP_WINDOW_WIDTH = 331;
        private const int APP_WINDOW_HEIGHT = 419;

        public App()
        {
            InitializeComponent();

            MainPage = new AppShell();
        }

        protected override Window CreateWindow(IActivationState? activationState)
        {
            var window = base.CreateWindow(activationState);

            // https://putridparrot.com/blog/maui-setting-the-apps-dimensions-on-a-windows-desktop/
            #if WINDOWS
            if (DeviceInfo.Idiom == DeviceIdiom.Desktop)
            {
                window.Width = APP_WINDOW_WIDTH;
                window.Height = APP_WINDOW_HEIGHT;
            }
            #endif

            return window;
        }
    }
}
