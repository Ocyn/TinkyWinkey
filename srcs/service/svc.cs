using System.ServiceProcess;
using System.Timers;

public class MonService : ServiceBase
{
    private Timer timer;

    public MonService()
    {
        this.ServiceName = "MonServiceWindows";
    }

    protected override void OnStart(string[] args)
    {
        // Code pour démarrer le service
        timer = new Timer();
        timer.Interval = 60000; // 60 secondes
        timer.Elapsed += new ElapsedEventHandler(this.OnTimer);
        timer.Start();
    }

    private void OnTimer(object sender, ElapsedEventArgs e)
    {
        // Code à exécuter périodiquement
    }

    protected override void OnStop()
    {
        // Code pour arrêter le service
        timer.Stop();
    }

    static void Main()
    {
        ServiceBase[] ServicesToRun;
        ServicesToRun = new ServiceBase[]
        {
            new MonService()
        };
        ServiceBase.Run(ServicesToRun);
    }
}
