class SchanaPartyMenu extends UIScriptedMenu {
	protected bool m_SchanaPartyIsInitialized;
	protected bool m_SchanaPartyMenuIsOpen;
	protected ref DBMPartySettings 		m_clientSettings;
	protected TextListboxWidget 		m_SchanaPartyPlayerList;
	protected TextListboxWidget 		m_SchanaPartyPartyList;
	protected ButtonWidget 				m_SchanaPartyButtonAdd;
	protected ButtonWidget 				m_SchanaPartyButtonRemove;
	protected ButtonWidget 				m_BtnPlayersTab;
	protected ButtonWidget				m_BtnPartyTab;
	protected ButtonWidget 				m_BtnSettingsTab;
	protected ButtonWidget				m_BtnOpenDiscord;
	protected ButtonWidget				m_BtnResetSettings;
	protected EditBoxWidget				m_PlayerSearchBox;
	protected EditBoxWidget				m_MarkerRangeBox;
	protected EditBoxWidget				m_NameTagRangeBox;
	protected EditBoxWidget				m_MaxPlayersOnScreenBox;
	protected Widget					m_PlayersTab;
	protected Widget					m_PartyTab;
	protected Widget					m_SettingsTab;
	protected TextWidget				m_PlayersOnlineValue;
	protected SliderWidget 				m_MarkerColorR;
	protected SliderWidget 				m_MarkerColorG;
	protected SliderWidget 				m_MarkerColorB;
	protected SliderWidget 				m_NameTagColorR;
	protected SliderWidget 				m_NameTagColorG;
	protected SliderWidget 				m_NameTagColorB;
	protected SliderWidget 				m_NameTagOperacity;
	protected XComboBoxWidget 			m_nameTagStyle;
	protected CheckBoxWidget 			m_DisableTacticalPing;
	protected CheckBoxWidget 			m_DisablePartyNotify;

	protected ref map<string, string> member_sorting_map;

	//SearchFunction:
	const float m_FilterUpdate = 0.35;
	float m_FilterUpdateCurTick;
	private string 	m_searchFilter;
	bool isSearching;

	void SchanaPartyMenu () {
		member_sorting_map = new map<string, string> ();
		m_clientSettings = GetDBMPartySettings();
	}

	override Widget Init () {
		if (!m_SchanaPartyIsInitialized) {
			layoutRoot = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModParty/GUI/Layouts/DBM.layout");
			m_SchanaPartyPlayerList 	= TextListboxWidget.Cast (layoutRoot.FindAnyWidget ("Players"));
			m_SchanaPartyPartyList 		= TextListboxWidget.Cast (layoutRoot.FindAnyWidget ("Party"));
			m_SchanaPartyButtonAdd 		= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("ButtonAdd"));
			m_SchanaPartyButtonRemove 	= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("ButtonRemove"));
			m_BtnPlayersTab 			= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("BtnPlayersTab0"));
			m_BtnPartyTab				= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("BtnPartyTab"));
			m_BtnSettingsTab 			= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("BtnSettingsTab"));
			m_BtnOpenDiscord			= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("BtnDiscord"));
			m_BtnResetSettings			= ButtonWidget.Cast (layoutRoot.FindAnyWidget ("ResetSettingsBtn"));
			m_PlayerSearchBox 			= EditBoxWidget.Cast (layoutRoot.FindAnyWidget ("PlayerSearchBox"));
			m_MarkerRangeBox			= EditBoxWidget.Cast (layoutRoot.FindAnyWidget ("MarkerRangeInput"));
			m_NameTagRangeBox			= EditBoxWidget.Cast (layoutRoot.FindAnyWidget ("NameTagRangeInput"));
			m_MaxPlayersOnScreenBox		= EditBoxWidget.Cast (layoutRoot.FindAnyWidget ("MaxPlayersInput"));
			m_PlayersTab				= Widget.Cast (layoutRoot.FindAnyWidget ("PlayersTab"));
			m_SettingsTab				= Widget.Cast (layoutRoot.FindAnyWidget ("SettingsTab"));
			m_PartyTab					= Widget.Cast (layoutRoot.FindAnyWidget ("PartyTab"));
			m_PlayersOnlineValue		= TextWidget.Cast (layoutRoot.FindAnyWidget ("PlayersOnlineValue"));
			m_MarkerColorR				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("MarkerR"));
			m_MarkerColorG				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("MarkerG"));
			m_MarkerColorB				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("MarkerB"));
			m_NameTagColorR				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("NameTagR"));
			m_NameTagColorG				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("NameTagG"));
			m_NameTagColorB				= SliderWidget.Cast (layoutRoot.FindAnyWidget ("NameTagB"));
			m_NameTagOperacity			= SliderWidget.Cast (layoutRoot.FindAnyWidget ("NameTagOP"));
			m_nameTagStyle				= XComboBoxWidget.Cast (layoutRoot.FindAnyWidget ("SelectNametagStyle"));
			m_DisableTacticalPing		= CheckBoxWidget.Cast (layoutRoot.FindAnyWidget ("CheckBoxtacticalPing"));
			m_DisablePartyNotify		= CheckBoxWidget.Cast (layoutRoot.FindAnyWidget ("CheckBoxTacticalPing"));
			m_SchanaPartyIsInitialized 	= true;
		}

		SchanaPartyUpdateLists ();

		return layoutRoot;
	}

	void ~SchanaPartyMenu () {
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaPartyUpdateLists);

		GetGame ().GetUIManager ().Back ();
		g_Game.GetUIManager ().ShowCursor (true);
		g_Game.GetUIManager ().ShowUICursor (false);
		GetGame ().GetInput ().ResetGameFocus ();
		isSearching = false;
		if (layoutRoot) {
			layoutRoot.Unlink ();
		}
	}

	override void OnShow () {
		super.OnShow ();
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).CallLater (this.SchanaPartyUpdateLists, 500, true);
	}

	override void OnHide () {
		super.OnHide ();
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaPartyUpdateLists);

		g_Game.GetUIManager ().ShowCursor (true);
		g_Game.GetUIManager ().ShowUICursor (false);
		GetGame ().GetInput ().ResetGameFocus ();
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if(w == m_MarkerColorR)
		{
			if(finished)
				m_clientSettings.UpdateMarkerColorR (m_MarkerColorR.GetCurrent ());
			return true;
		}
		else if(w == m_MarkerColorG)
		{
			if(finished)
				m_clientSettings.UpdateMarkerColorG (m_MarkerColorG.GetCurrent ());
			return true;
		}
		else if(w == m_MarkerColorB)
		{
			if(finished)
				m_clientSettings.UpdateMarkerColorB (m_MarkerColorB.GetCurrent ());
			return true;
		}
		else if(w == m_NameTagColorR)
		{
			if(finished)
				m_clientSettings.UpdateNameTagColorR (m_NameTagColorR.GetCurrent ());
			return true;
		}
		else if(w == m_NameTagColorG)
		{
			if(finished)
				m_clientSettings.UpdateNameTagColorG (m_NameTagColorG.GetCurrent ());
			return true;
		}
		else if(w == m_NameTagColorB)
		{
			if(finished)
				m_clientSettings.UpdateNameTagColorB (m_NameTagColorB.GetCurrent ());
			return true;
		}
		else if(w == m_NameTagOperacity)
		{
			if(finished)
				m_clientSettings.UpdateMarkerOperacity (m_NameTagOperacity.GetCurrent ());
			return true;
		}
		else if(w == m_nameTagStyle)
		{
			if(finished)
				m_clientSettings.UpdateNameTagStyle (m_nameTagStyle.GetCurrentItem ());
				m_clientSettings.UpdateNeedAnUpdate(true);
		}
		else if(w == m_MarkerRangeBox)
		{
			if(finished){
				if(IsNumberOnly(m_MarkerRangeBox.GetText())){
				m_clientSettings.UpdateMarkerRenderRange (m_MarkerRangeBox.GetText().ToInt());
				}else{
					m_MarkerRangeBox.SetText("");
				}
			}
		}
		else if(w == m_NameTagRangeBox)
		{
			if(finished){
				if(IsNumberOnly(m_NameTagRangeBox.GetText())){
					m_clientSettings.UpdateNameTagRenderRange (m_NameTagRangeBox.GetText().ToInt());
				}else{
					m_NameTagRangeBox.SetText("");
				}
			}
		}
		else if(w == m_MaxPlayersOnScreenBox)
		{
			if(finished){
				if(IsNumberOnly(m_MaxPlayersOnScreenBox.GetText())){
					m_clientSettings.UpdateMaxPlayersOnScreen (m_MaxPlayersOnScreenBox.GetText().ToInt());
				}else{
					m_MaxPlayersOnScreenBox.SetText("");
				}
			}
		}
		else if(w == m_PlayerSearchBox){
			SearchPlayer();
		}
		return super.OnChange(w, x, y, finished);
	}

	override bool OnClick (Widget w, int x, int y, int button) {
		int selectedRow;
		Param1<string> id;

		switch (w) 
		{
			case m_SchanaPartyButtonAdd:
				selectedRow = m_SchanaPartyPlayerList.GetSelectedRow ();
				if (selectedRow == -1) {
					break;
				}
				m_SchanaPartyPlayerList.GetItemData (selectedRow, 0, id);
				SchanaPartyUtils.LogMessage ("Menu add " + id.param1);
				auto data = new Param1<string> (id.param1);
        		GetRPCManager ().SendRPC ("SchanaModParty", "MessageAddPlayer", data);
				GetSchanaPartyManagerClient ().AddPlayerToParty (id.param1);
				SchanaPartyUpdateLists ();
				break;

			case m_SchanaPartyButtonRemove:
				selectedRow = m_SchanaPartyPartyList.GetSelectedRow ();
				if (selectedRow == -1) {
					break;
				}
				m_SchanaPartyPartyList.GetItemData (selectedRow, 0, id);
				SchanaPartyUtils.LogMessage ("Menu remove " + id.param1);
				GetSchanaPartyManagerClient ().RemovePlayerFromParty (id.param1);
				SchanaPartyUpdateLists ();
				m_SchanaPartyPartyList.SelectRow (selectedRow - 1);
				break;
			case m_BtnPlayersTab:
				SwitchPartyTab (1);
				break;
			case m_BtnPartyTab:
				SwitchPartyTab (2);
				break;
			case m_BtnSettingsTab:
				SwitchPartyTab (3);
				break;
			case m_DisablePartyNotify:
				m_clientSettings.UpdateDisablePartyNotify (m_DisablePartyNotify.IsChecked ());
				break;
			case m_DisableTacticalPing:
				m_clientSettings.UpdateDisablePing (m_DisableTacticalPing.IsChecked ());
				break;
			case m_BtnOpenDiscord:
				GetGame().OpenURL("https://discord.gg/dJwuvcT");
				break;
			case m_BtnResetSettings:
				m_clientSettings.ResetPartySettings();
				DrawLoadetSettings();
				break;
		}
		return super.OnClick (w, x, y, button);
	}
	
	void SwitchPartyTab (int TabIndex) {

		switch(TabIndex){
			case 1:
				m_PlayersTab.Show (true);
				m_PartyTab.Show (false);
				m_SettingsTab.Show (false);
			break;

			case 2:
				m_PlayersTab.Show (false);
				m_PartyTab.Show (true);
				m_SettingsTab.Show (false);
			break;

			case 3:
				m_PlayersTab.Show (false);
				m_PartyTab.Show (false);
				m_SettingsTab.Show (true);
			break;
		}
	}

	void SchanaPartyUpdateLists () {
		if (m_SchanaPartyIsInitialized) {
			SchanaPartyUpdatePartyList ();
			SchanaPartyUpdatePlayerList ();
			SchanaPartyUpdatePartyStatus ();
		}
	}

	void SchanaPartyUpdatePartyList () {
		member_sorting_map.Clear ();
		string member_name;
		int insert_row = 0;

		auto members = GetSchanaPartySettings ().GetMembers ();
		foreach (auto id : members) {
			member_name = GetSchanaPartySettings ().GetName (id);
			member_name.ToLower ();
			member_sorting_map.Insert (member_name + id, id);
		}

		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		foreach (auto sorted_key : sorted_keys) {
			string member_id = member_sorting_map.Get (sorted_key);
			member_name = GetSchanaPartySettings ().GetName (member_id);
			if (insert_row < m_SchanaPartyPartyList.GetNumItems ()) {
				m_SchanaPartyPartyList.SetItem (insert_row, member_name, new Param1<string> (member_id), 0);
			} else {
				m_SchanaPartyPartyList.AddItem (member_name, new Param1<string> (member_id), 0);
			}
			insert_row++;
		}

		while (insert_row < m_SchanaPartyPartyList.GetNumItems ()) {
			m_SchanaPartyPartyList.SelectRow (-1);
			m_SchanaPartyPartyList.RemoveRow (insert_row);
		}

		if (m_SchanaPartyPartyList.GetSelectedRow () >= insert_row) {
			m_SchanaPartyPartyList.SelectRow (insert_row - 1);
		}
	}

	void SchanaPartyUpdatePlayerList () {
		if(m_PlayerSearchBox.GetText() != "")
			return;
		member_sorting_map.Clear ();
		int insert_row = 0;
		auto onlinePlayers = GetSchanaPartyManagerClient ().GetOnlinePlayers ();
		foreach (auto id, auto player_name : onlinePlayers) {
			player_name.ToLower ();
			member_sorting_map.Insert (player_name + id, id);
		}

		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		foreach (auto sorted_key : sorted_keys) {
			string player_id = member_sorting_map.Get (sorted_key);

			if (insert_row < m_SchanaPartyPlayerList.GetNumItems ()) {
				m_SchanaPartyPlayerList.SetItem (insert_row, onlinePlayers.Get (player_id), new Param1<string> (player_id), 0);
			} else {
				m_SchanaPartyPlayerList.AddItem (onlinePlayers.Get (player_id), new Param1<string> (player_id), 0);
			}
			insert_row++;
		}

		while (insert_row < m_SchanaPartyPlayerList.GetNumItems ()) {
			m_SchanaPartyPlayerList.RemoveRow (insert_row);
		}

		if (m_SchanaPartyPlayerList.GetSelectedRow () >= insert_row) {
			m_SchanaPartyPlayerList.SelectRow (insert_row - 1);
		}
		local int serverPlayerCount = m_SchanaPartyPlayerList.GetNumItems ();
		serverPlayerCount + 1;
		m_PlayersOnlineValue.SetText(serverPlayerCount.ToString ());
	}

	void SchanaPartyUpdatePartyStatus () {
		Param1<string> id;
		for (int i = 0; i < m_SchanaPartyPartyList.GetNumItems (); ++i) {
			m_SchanaPartyPartyList.GetItemData (i, 0, id);

			if (GetSchanaPartyManagerClient ().IsPartyMemberOnline (id.param1)) {
				// Green A400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFF00E676);
			} else if (GetSchanaPartyManagerClient ().IsPartyMemberOnlineButNotMutual (id.param1)) {
				// Light Blue A400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFF00B0FF);
			} else {
				// Gray 400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFFBDBDBD);
			}
		}
	}
	void DrawLoadetSettings (){
		m_MarkerColorR.SetCurrent(m_clientSettings.GetMarkerColorR());
		m_MarkerColorG.SetCurrent(m_clientSettings.GetMarkerColorG());
		m_MarkerColorB.SetCurrent(m_clientSettings.GetMarkerColorB());

		m_NameTagColorR.SetCurrent(m_clientSettings.GetNameTagColorR());
		m_NameTagColorG.SetCurrent(m_clientSettings.GetNameTagColorG());
		m_NameTagColorB.SetCurrent(m_clientSettings.GetNameTagColorB());

		m_NameTagOperacity.SetCurrent(m_clientSettings.GetMarkerOperacity());

		m_MarkerRangeBox.SetText(m_clientSettings.GetMarkerRenderRange().ToString());
		m_NameTagRangeBox.SetText(m_clientSettings.GetNameTagRenderRange().ToString());
		m_DisableTacticalPing.SetChecked(m_clientSettings.IsTacticalPingDisabled());
		m_DisablePartyNotify.SetChecked(m_clientSettings.IsPartyNotifyDisabled());
		m_nameTagStyle.SetCurrentItem(m_clientSettings.GetNameTagSytle());
	}

	bool SchanaPartyMenuIsOpen () {
		return m_SchanaPartyMenuIsOpen;
	}

	void SearchPlayer()
	{
		string searchText = m_PlayerSearchBox.GetText();
		if(searchText == ""){
			SchanaPartyUpdatePlayerList();
			return;
		}
		searchText.ToLower();
		m_SchanaPartyPlayerList.ClearItems();	
		member_sorting_map.Clear ();
		auto onlinePlayers = GetSchanaPartyManagerClient().GetOnlinePlayers();
		foreach (auto id, auto player_name : onlinePlayers) 
		{
			//player_name.ToLower();
			string searchedPlayer = player_name;
			searchedPlayer.ToLower();
			if(searchedPlayer.Contains(searchText))
			{
				m_SchanaPartyPlayerList.AddItem (player_name, new Param1<string> (id), 0);
			}
		}
	}

	bool ContainsNumber(string index)
	{
		if(index.Contains("1") || index.Contains("2") || index.Contains("3") || index.Contains("4") || index.Contains("5") || index.Contains("6") || index.Contains("7") || index.Contains("8") || index.Contains("9") || index.Contains("0"))
			return true;
		return false;	
	}

	protected bool IsNumberOnly(string input)
    {
		for(int i = 0; i < input.Length(); i++)
		{
			if(!ContainsNumber(input.Get(i)))
			{
				return false;
			}
		}
		return true;
    }

	void SchanaPartyMenuSetOpen (bool open) {
		m_SchanaPartyMenuIsOpen = open;
	}
}