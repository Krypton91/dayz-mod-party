class SchanaModPartyClientDisplaySettings {
    protected static string DIR = "$profile:DeutscheBohrmaschine";
    protected static string PATH = DIR + "\\display_settings.json";

    protected bool enable_party = true;
    protected bool show_markers = true;

    protected vector nametag_color = "255 255 255";
    protected vector distance_color = "255 255 255";
    protected vector icon_color = "255 255 255";

    protected bool nametag_enabled = true;
    protected int nametag_distance_start = 0;
    protected int nametag_distance_end = 15000;

    protected bool distance_enabled = true;
    protected int distance_distance_start = 0;
    protected int distance_distance_end = 2000;

    protected bool icon_enabled = true;
    protected int icon_distance_start = 0;
    protected int icon_distance_end = 15000;

    protected int snap_distance = 2000;

    void Save () {
        if (GetGame ().IsClient ()) {
            if (!FileExist (DIR)) {
                MakeDirectory (DIR);
            }
            JsonFileLoader<SchanaModPartyClientDisplaySettings>.JsonSaveFile (PATH, this);
        }
    }

    static ref SchanaModPartyClientDisplaySettings Get () {

        auto settings = new ref SchanaModPartyClientDisplaySettings ();

        if (FileExist (PATH)) {
            JsonFileLoader<SchanaModPartyClientDisplaySettings>.JsonLoadFile (PATH, settings);
        }

        return settings;
    }
}

static ref SchanaModPartyClientDisplaySettings g_SchanaModPartyClientDisplaySettings;
static ref SchanaModPartyClientDisplaySettings GetSchanaModPartyClientDisplaySettings () {
    if (g_Game.IsClient () && !g_SchanaModPartyClientDisplaySettings) {
        g_SchanaModPartyClientDisplaySettings = SchanaModPartyClientDisplaySettings.Get ();
        g_SchanaModPartyClientDisplaySettings.Save ();
    }
    return g_SchanaModPartyClientDisplaySettings;
}