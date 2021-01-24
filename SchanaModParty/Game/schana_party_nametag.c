class SchanaPartyNametagsMenu extends UIScriptedMenu {
    static string SCHANA_PARTY_NAMETAG_DELETE = "SCHANA_PARTY_NAMETAG_DELETE";

    protected Widget m_SchanaPartyNametagRoot;
    protected TextWidget m_SchanaPartyNametagNametag;
    protected TextWidget m_SchanaPartyNametagDistance;
    protected ImageWidget m_SchanaPartyNametagIcon;

    protected Widget m_SchanaPartyListRootWidget;
    protected ref array<ImageWidget> m_SchanaPartyListHealthWidgets;
    protected TextWidget m_SchanaPartyListTextWidget;

    protected DayZPlayer m_SchanaPartyNametagPlayer;
    protected ref SchanaPartyMemberBasicMapMarkerHelper m_BasicMapHelper;
    protected vector m_SchanaPartyPlayerServerPosition = "0 0 0";
    protected float m_SchanaPartyPlayerServerHealth = 100;
    protected string m_SchanaPartyPlayerName = "";
    protected int m_SchanaPartyListIndex = 0;
    protected int m_MemberOnScreenCount = 0;
    protected ref DBMPartySettings m_clientSettings;

    void SchanaPartyNametagsMenu (DayZPlayer player) {
        m_SchanaPartyNametagRoot = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModParty/GUI/Layouts/nametag.layout");
        m_SchanaPartyNametagNametag = TextWidget.Cast (m_SchanaPartyNametagRoot.FindAnyWidget ("nametag"));
        m_SchanaPartyNametagDistance = TextWidget.Cast (m_SchanaPartyNametagRoot.FindAnyWidget ("distance"));
        m_SchanaPartyNametagIcon = ImageWidget.Cast (m_SchanaPartyNametagRoot.FindAnyWidget ("icon"));
        m_SchanaPartyNametagRoot.Show (false);
        m_clientSettings = GetDBMPartySettings();
        m_SchanaPartyListRootWidget = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModParty/GUI/Layouts/party.layout");
        m_SchanaPartyListTextWidget = TextWidget.Cast (m_SchanaPartyListRootWidget.FindAnyWidget ("Nametag"));
        m_SchanaPartyListHealthWidgets = new array<ImageWidget>;
        for (int i = 0; i < 15; ++i) {
            m_SchanaPartyListHealthWidgets.Insert (ImageWidget.Cast (m_SchanaPartyListRootWidget.FindAnyWidget ("IconHealth" + i.ToString ())));
        }

        m_SchanaPartyNametagPlayer = DayZPlayer.Cast (player);
        m_BasicMapHelper = new SchanaPartyMemberBasicMapMarkerHelper ();

        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).CallLater (this.SchanaUpdate, 16, true);
    }

    void ~SchanaPartyNametagsMenu () {
        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaUpdate);
        if (m_SchanaPartyNametagRoot != null) {
            m_SchanaPartyNametagRoot.Show (false);
            m_SchanaPartyNametagRoot.Unlink ();
        }

        if (m_SchanaPartyListRootWidget != null) {
            m_SchanaPartyListRootWidget.Show (false);
            m_SchanaPartyListRootWidget.Unlink ();
        }
        if (m_BasicMapHelper) {
            delete m_BasicMapHelper;
        }
    }

    void SchanaUpdate () {
        if (SchanaPartyNametagShouldShow ()) {
            SchanaPartyNametagUpdate ();
        } else if (m_SchanaPartyNametagRoot != null) {
            m_SchanaPartyNametagRoot.Show (false);
            delete this;
        }
    }

    protected bool SchanaPartyNametagShouldShow () {
        if (m_SchanaPartyNametagRoot == null) {
            return false;
        }
        if (!GetGame ().GetPlayer ()) {
            return false;
        }
        if (m_SchanaPartyPlayerName == SCHANA_PARTY_NAMETAG_DELETE) {
            return false;
        }
        return true;
    }

    protected vector SchanaPartyGetPlayerPosition () {
        if (m_SchanaPartyNametagPlayer && m_SchanaPartyNametagPlayer.IsAlive ()) {
            return m_SchanaPartyNametagPlayer.GetPosition ();
        } else {
            return m_SchanaPartyPlayerServerPosition;
        }
    }

    protected float SchanaPartyGetPlayerHealth () {
        return m_SchanaPartyPlayerServerHealth;
    }

    void SchanaPartyUpdatePosition (vector position) {
        m_SchanaPartyPlayerServerPosition = position;
    }

    void SchanaPartyUpdateHealth (float health) {
        m_SchanaPartyPlayerServerHealth = health;
    }

    void SchanaPartyUpdatePlayer (DayZPlayer player) {
        m_SchanaPartyNametagPlayer = DayZPlayer.Cast (player);
    }

    void SchanaPartyUpdateName (string name) {
        m_SchanaPartyPlayerName = name;
    }

    void SchanaPartyUpdateListIndex (int index) {
        m_SchanaPartyListIndex = index;
    }

    void SchanaPartySetRemoveFlag () {
        m_SchanaPartyPlayerName = SCHANA_PARTY_NAMETAG_DELETE;
    }

    protected string SchanaPartyGetPlayerName () {
        if (m_SchanaPartyNametagPlayer && m_SchanaPartyNametagPlayer.GetIdentity ()) {
            return m_SchanaPartyNametagPlayer.GetIdentity ().GetName ();
        } else {
            return m_SchanaPartyPlayerName;
        }
    }

    void SchanaPartyNametagUpdate () {
        float x, y, distance;
        vector position = SchanaPartyGetPlayerPosition ();
        vector screenPosition = GetGame ().GetScreenPos (position + "0 1.3 0");

        x = Math.Round (screenPosition[0]);
        y = Math.Round (screenPosition[1]);
        m_SchanaPartyNametagRoot.SetPos (x, y);

        distance = Math.Round (vector.Distance (position, GetGame ().GetPlayer ().GetPosition ()));
        string distanceString = distance.ToString () + "m";
        if (distance > 1000) {
            distanceString = (Math.Round (distance / 100) / 10).ToString () + "km";
        }
        string text = SchanaPartyGetPlayerName () + " " + distanceString;
        SchanaPartyListUpdate (text);
        CheckNameTag ();

        if(isOutOfRenderRange(distance)) {
            m_SchanaPartyNametagRoot.Show(false);
        }else{
            m_SchanaPartyNametagNametag.SetText (SchanaPartyGetPlayerName ());
            m_SchanaPartyNametagDistance.SetText (distanceString);
            m_SchanaPartyNametagRoot.Show (SchanaPartyNametagVisibleOnScreen ());
        }

        m_BasicMapHelper.SetPosition (position);
        m_BasicMapHelper.SetName (SchanaPartyGetPlayerName ());
    }

    void SchanaPartyListUpdate (string text) {
            m_SchanaPartyListTextWidget.SetText (text);
            float health = SchanaPartyGetPlayerHealth () * 0.01;
            int healthLevel = 4 - health * 4;
            for (int i = 0; i < 15; ++i) {
                if(m_SchanaPartyListHealthWidgets.Get (i))
                    m_SchanaPartyListHealthWidgets.Get (i).Show (healthLevel == i);
            }
            float width, height, x, y;
            m_SchanaPartyListRootWidget.GetSize (width, height);
            m_SchanaPartyListRootWidget.GetPos (x, y);
            y = (5 + height) * m_SchanaPartyListIndex;
            m_SchanaPartyListRootWidget.SetPos (x, y);
    }
    
    void CheckNameTag () {
            m_SchanaPartyNametagNametag.SetColor(m_clientSettings.GetHexaNameTagColor ());
            m_SchanaPartyNametagDistance.SetColor(m_clientSettings.GetHexaNameTagColor ());
            m_SchanaPartyNametagIcon.SetColor(m_clientSettings.GetHexaNameTagColor ());
            m_SchanaPartyNametagNametag.SetAlpha(m_clientSettings.GetMarkerOperacity ());
            m_SchanaPartyNametagDistance.SetAlpha(m_clientSettings.GetMarkerOperacity ());
            m_SchanaPartyNametagIcon.SetAlpha(m_clientSettings.GetMarkerOperacity ());
            if(m_clientSettings.NeedAnUpdate()){
                switch(m_clientSettings.GetNameTagSytle ()) {
                    case 0:
                        m_SchanaPartyNametagNametag.Show(true);
                        m_SchanaPartyNametagDistance.Show(true);
                        m_SchanaPartyNametagIcon.Show(true);
                        if(m_SchanaPartyNametagRoot == null);
                            m_SchanaPartyNametagRoot.Show(true);
                        break;

                    case 1:
                        m_SchanaPartyNametagNametag.Show(true);
                        m_SchanaPartyNametagDistance.Show(false);
                        m_SchanaPartyNametagIcon.Show(false);
                        if(m_SchanaPartyNametagRoot == null);
                            m_SchanaPartyNametagRoot.Show(true);
                        break;
                    
                    case 2:
                        m_SchanaPartyNametagNametag.Show(true);
                        m_SchanaPartyNametagDistance.Show(true);
                        m_SchanaPartyNametagIcon.Show(false);
                        if(m_SchanaPartyNametagRoot == null);
                            m_SchanaPartyNametagRoot.Show(true);
                        break;
                    
                    case 3:
                        m_SchanaPartyNametagNametag.Show(false);
                        m_SchanaPartyNametagDistance.Show(false);
                        m_SchanaPartyNametagIcon.Show(false);
                        m_SchanaPartyNametagRoot.Show(false);
                        break;
                }
                m_clientSettings.UpdateNeedAnUpdate(false);
            }
    }

    /* Gets triggert if player is out of render range defined in settings */ 
    protected bool isOutOfRenderRange (float distance) {
        if(distance >= m_clientSettings.GetNameTagRenderRange() && m_clientSettings.GetNameTagRenderRange() != -1){
            return true;
        }else{
            return false;
        }
    }
    protected bool SchanaPartyNametagVisibleOnScreen () {
        vector position = SchanaPartyGetPlayerPosition ();
        vector screenPositionRelative = GetGame ().GetScreenPosRelative (position);

        if (screenPositionRelative[0] >= 1 || screenPositionRelative[0] == 0 || screenPositionRelative[1] >= 1 || screenPositionRelative[1] == 0) {
            return false;
        } else if (screenPositionRelative[2] < 0) {
            return false;
        }
        return true;
    }
}