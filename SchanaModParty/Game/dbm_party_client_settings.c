class DBMPartySettings {
    protected static string DIR = "$profile:DeutscheBohrmaschine";
    protected static string PATH = DIR + "\\PlayerSettings.json";

    protected int MarkerColor_R;
    protected int MarkerColor_G;
    protected int MarkerColor_B;
    protected int NameTagColor_R;
    protected int NameTagColor_G;
    protected int NameTagColor_B;
    protected int MarkerRenderRange;
    protected int NameTagRenderRange;
    protected bool DisableTacticalPing;
    protected bool DisablePartyNotify;
    protected int MaxPlayersOnScreen;
    protected int MarkerOperacity;
    protected int NameTagStyle;

    void DBMPartySettings () {

    }

    void Save () {
        if (GetGame ().IsClient ()) {
            if (!FileExist (DIR)) {
                MakeDirectory (DIR);
            }
            JsonFileLoader<DBMPartySettings>.JsonSaveFile (PATH, this);
            m_NeedsAnUpdate = true;
        }
    }
    protected bool m_NeedsAnUpdate = false;
    /* Getter */
    int GetHexaMarkerColor () {
        return ARGB(255, MarkerColor_R, MarkerColor_G, MarkerColor_B);
    }

    int GetHexaNameTagColor () {
        return ARGB(255, NameTagColor_R, NameTagColor_G, NameTagColor_B);
    }

    float GetMarkerColorR () {
        string tempColor = MarkerColor_R.ToString();
        return tempColor.ToFloat();
    }

    float GetMarkerColorG () {
        string tempColor = MarkerColor_G.ToString();
        return tempColor.ToFloat();
    }

    float GetMarkerColorB () {
        string tempColor = MarkerColor_B.ToString();
        return tempColor.ToFloat();
    }

    float GetNameTagColorR (){
        string tempColor = NameTagColor_R.ToString();
        return tempColor.ToFloat();
    }

    float GetNameTagColorG (){
        string tempColor = NameTagColor_G.ToString();
        return tempColor.ToFloat();
    }

    float GetNameTagColorB (){
        string tempColor = NameTagColor_B.ToString();
        return tempColor.ToFloat();
    }

    float GetMarkerRenderRange () {
        string tempRange = MarkerRenderRange.ToString();
        return tempRange.ToFloat();
    }

    float GetNameTagRenderRange () {
        string tempRenderRange = NameTagRenderRange.ToString();
        return tempRenderRange.ToFloat();
    }
    bool IsTacticalPingDisabled () {
        return DisableTacticalPing;
    }

    bool IsPartyNotifyDisabled () {
        return DisablePartyNotify;
    }

    int GetMaxPlayersOnScreen () {
        return MaxPlayersOnScreen;
    }

    float GetMarkerOperacity () {
        string tempOP = MarkerOperacity.ToString();
        return tempOP.ToFloat();
    }

    int GetNameTagSytle () {
        return NameTagStyle;
    }

    /* Setter */
    void UpdateMarkerColorR (int R) {
        MarkerColor_R = R;
        Save();
    }

    void UpdateMarkerColorG (int G) {
        MarkerColor_G = G;
    }

    void UpdateMarkerColorB (int B) {
        MarkerColor_B = B;
        Save();
    }

    void UpdateNameTagColorR (int R) {
        NameTagColor_R = R;
        Save();
    }

    void UpdateNameTagColorG (int G) {
        NameTagColor_G = G;
        Save();
    }

    void UpdateNameTagColorB (int B) {
        NameTagColor_B = B;
        Save();
    }

    void UpdateMarkerRenderRange (int RenderRange) {
        MarkerRenderRange = RenderRange;
        Save();
    }

    void UpdateNameTagRenderRange (int RenderRange) {
        NameTagRenderRange = RenderRange;
        Save();
    }

    void UpdateDisablePing (bool state) {
        DisableTacticalPing = state;
        Save();
    }

    void UpdateDisablePartyNotify (bool state) {
        DisablePartyNotify = state;
        Save();
    }

    void UpdateMaxPlayersOnScreen (int MaxPlayer) {
        MaxPlayersOnScreen = MaxPlayer;
        Save();
    }

    void UpdateMarkerOperacity (int operacity) {
        MarkerOperacity = operacity;
        Save();
    }

    void UpdateNameTagStyle (int nametagstyle) {
        NameTagStyle = nametagstyle;
        Save();
    }

    void UpdateNeedAnUpdate (bool state) {
        m_NeedsAnUpdate = state;
    }

    void ResetPartySettings(){
        MarkerColor_B = 255;
        MarkerColor_G = 255;
        MarkerColor_B = 0;
        NameTagColor_R = 0;
        NameTagColor_G = 255;
        NameTagColor_B = 0;
        MarkerRenderRange = -1;
        NameTagRenderRange = -1;
        DisableTacticalPing = false;
        DisablePartyNotify = false;
        MaxPlayersOnScreen = 5;
        MarkerOperacity = 1;
        NameTagStyle = 0;
        JsonFileLoader<DBMPartySettings>.JsonSaveFile (PATH, this);
        UpdateNeedAnUpdate(true);
    }
    static ref DBMPartySettings LoadSettings () {

        auto settings = new ref DBMPartySettings ();

        if (FileExist (PATH)) {
            JsonFileLoader<DBMPartySettings>.JsonLoadFile (PATH, settings);
        }else{
            settings = DBMPartyLoadDefaultSettings(settings);
            JsonFileLoader<DBMPartySettings>.JsonSaveFile (PATH, settings);
        }

        return settings;
    }

    bool NeedAnUpdate () {
        return m_NeedsAnUpdate;
    }

    static DBMPartySettings DBMPartyLoadDefaultSettings (DBMPartySettings settings) {
        settings.MarkerColor_B = 255;
        settings.MarkerColor_G = 255;
        settings.MarkerColor_B = 0;
        settings.NameTagColor_R = 0;
        settings.NameTagColor_G = 255;
        settings.NameTagColor_B = 0;
        settings.MarkerRenderRange = -1;
        settings.NameTagRenderRange = -1;
        settings.DisableTacticalPing = false;
        settings.DisablePartyNotify = false;
        settings.MaxPlayersOnScreen = 5;
        settings.MarkerOperacity = 1;
        settings.NameTagStyle = 0;
        return settings;
    }
}

static ref DBMPartySettings g_DBMPartySettings;
static ref DBMPartySettings GetDBMPartySettings () {
    if (g_Game.IsClient () && !g_DBMPartySettings) {
        g_DBMPartySettings = DBMPartySettings.LoadSettings ();
        g_DBMPartySettings.Save ();
    }
    return g_DBMPartySettings;
}