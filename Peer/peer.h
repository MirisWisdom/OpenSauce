////////////////////////////////////////////////////////////////////////////
// File:	peer.h
// SDK:		GameSpy Peer SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _PEER_H_
#define _PEER_H_

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Public SDK Interface
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#include "../common/gsCommon.h"
#include "../Chat/chat.h"
#include "../qr2/qr2.h"
#include "../serverbrowsing/sb_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Defines for the msg param that's passed into peerListingGamesCallback().
#define PEER_ADD        0  // a server is being added
#define PEER_UPDATE     1  // a server has been updated
#define PEER_REMOVE     2  // a server has been removed
#define PEER_CLEAR      3  // all the servers have been cleared
#define PEER_COMPLETE   4  // the initial listing of servers is complete

////////////////////////////////////////////////////////////////////////////
// Nick errors, for peerNickErrorCallback().
#define PEER_NICK_OK                0
#define PEER_IN_USE                 1  // the nick is already being used
#define PEER_INVALID                2  // the nick contains invalid characters
#define PEER_UNIQUENICK_EXPIRED     3  // the uniquenick for this account has expired
#define PEER_NO_UNIQUENICK          4  // there is no uniquenick for this account
#define PEER_INVALID_UNIQUENICK     5  // the uniquenick to associate with the account is invalid or in use
#define PEER_NICK_TOO_LONG          6  // the nick was too long 
	
///////////////////////////////////////////////////////////
// Possible values for the failureReason passed to the 
// peerConnectCallback().
#define PEER_DISCONNECTED           0  // disconnected from, or unable to connect to, the server
#define PEER_NICK_ERROR             1  // a nick error was either ignored or not handled
#define PEER_LOGIN_FAILED           2  // the login info passed to peerConnectLogin was not valid

///////////////////////////////////////////////////////////
// Maximum length of a room password, including the 
// terminating NULL.
#define PEER_PASSWORD_LEN     24

///////////////////////////////////////////////////////////
// Each player can have various flags set for each room they 
// are in.
#define PEER_FLAG_STAGING     0x01  // s
#define PEER_FLAG_READY       0x02  // r
#define PEER_FLAG_PLAYING     0x04  // g
#define PEER_FLAG_AWAY        0x08  // a
#define PEER_FLAG_HOST        0x10  // h
#define PEER_FLAG_OP          0x20
#define PEER_FLAG_VOICE       0x40

///////////////////////////////////////////////////////////
// Bitfield reporting options for peerStartGame().
#define PEER_KEEP_REPORTING      0  // Continue reporting.
#define PEER_STOP_REPORTING      1  // Stop reporting.  Cannot be used with other options.
#define PEER_REPORT_INFO         2  // Continue reporting server keys (as if it were not playing).
#define PEER_REPORT_PLAYERS      4  // Continue reporting player keys (as if it were not playing).

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Function Name Mappings for Unicode Support
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#ifndef GSI_UNICODE
#define peerConnect						peerConnectA
#define peerConnectLogin				peerConnectLoginA
#define peerConnectPreAuth				peerConnectPreAuthA
#define peerRetryWithNick				peerRetryWithNickA
#define peerRegisterUniqueNick			peerRegisterUniqueNickA
#define peerSetTitle					peerSetTitleA
#define peerGetTitle					peerGetTitleA
#define peerGetNick						peerGetNickA
#define peerFixNick						peerFixNickA
#define peerTranslateNick				peerTranslateNickA
#define peerChangeNick					peerChangeNickA
#define peerSetAwayMode					peerSetAwayModeA
#define peerParseQuery					peerParseQueryA
#define peerAuthenticateCDKey			peerAuthenticateCDKeyA
#define peerJoinTitleRoom				peerJoinTitleRoomA
#define peerJoinStagingRoom				peerJoinStagingRoomA
#define peerJoinStagingRoomByChannel	peerJoinStagingRoomByChannelA
#define peerCreateStagingRoom			peerCreateStagingRoomA
#define peerCreateStagingRoomWithSocket	peerCreateStagingRoomWithSocketA
#define peerLeaveRoom					peerLeaveRoomA
#define peerListGroupRooms				peerListGroupRoomsA
#define peerStartListingGames			peerStartListingGamesA
#define peerMessageRoom					peerMessageRoomA
#define peerUTMRoom						peerUTMRoomA
#define peerSetPassword					peerSetPasswordA
#define peerSetRoomName					peerSetRoomNameA
#define peerGetRoomName					peerGetRoomNameA
#define peerGetRoomChannel				peerGetRoomChannelA
#define peerSetTitleRoomChannel			peerSetTitleRoomChannelA
#define peerMessagePlayer				peerMessagePlayerA
#define peerUTMPlayer					peerUTMPlayerA
#define peerKickPlayer					peerKickPlayerA
#define peerGetPlayerPing				peerGetPlayerPingA
#define peerGetPlayersCrossPing			peerGetPlayersCrossPingA
#define peerPingPlayer					peerPingPlayerA 
#define peerGetPlayerInfoNoWait			peerGetPlayerInfoNoWaitA
#define peerGetPlayerInfo				peerGetPlayerInfoA
#define peerIsPlayerHost				peerIsPlayerHostA
#define peerGetPlayerFlags				peerGetPlayerFlagsA
#define peerGetReady					peerGetReadyA
#define peerStartGame					peerStartGameA
#define peerSetGlobalKeys				peerSetGlobalKeysA
#define peerGetPlayerGlobalKeys			peerGetPlayerGlobalKeysA
#define peerGetRoomGlobalKeys			peerGetRoomGlobalKeysA
#define peerSetRoomKeys					peerSetRoomKeysA
#define peerGetRoomKeys					peerGetRoomKeysA
#define peerSetGlobalWatchKeys			peerSetGlobalWatchKeysA
#define peerSetRoomWatchKeys			peerSetRoomWatchKeysA
#define peerGetGlobalWatchKey			peerGetGlobalWatchKeyA
#define peerGetRoomWatchKey				peerGetRoomWatchKeyA
#define peerStartAutoMatch				peerStartAutoMatchA
#define peerStartAutoMatchWithSocket	peerStartAutoMatchWithSocketA
#else
#define peerConnect						peerConnectW
#define peerConnectLogin				peerConnectLoginW
#define peerConnectPreAuth				peerConnectPreAuthW
#define peerRetryWithNick				peerRetryWithNickW
#define peerRegisterUniqueNick			peerRegisterUniqueNickW
#define peerSetTitle					peerSetTitleW
#define peerGetTitle					peerGetTitleW
#define peerGetNick						peerGetNickW
#define peerFixNick						peerFixNickW
#define peerTranslateNick				peerTranslateNickW
#define peerChangeNick					peerChangeNickW
#define peerSetAwayMode					peerSetAwayModeW
#define peerParseQuery					peerParseQueryA
#define peerAuthenticateCDKey			peerAuthenticateCDKeyW
#define peerJoinTitleRoom				peerJoinTitleRoomW
#define peerJoinStagingRoom				peerJoinStagingRoomW
#define peerJoinStagingRoomByChannel	peerJoinStagingRoomByChannelW
#define peerCreateStagingRoom			peerCreateStagingRoomW
#define peerCreateStagingRoomWithSocket	peerCreateStagingRoomWithSocketW
#define peerLeaveRoom					peerLeaveRoomW
#define peerListGroupRooms				peerListGroupRoomsW
#define peerStartListingGames			peerStartListingGamesW
#define peerMessageRoom					peerMessageRoomW
#define peerUTMRoom						peerUTMRoomW
#define peerSetPassword					peerSetPasswordW
#define peerSetRoomName					peerSetRoomNameW
#define peerGetRoomName					peerGetRoomNameW
#define peerGetRoomChannel				peerGetRoomChannelW
#define peerSetTitleRoomChannel			peerSetTitleRoomChannelW
#define peerMessagePlayer				peerMessagePlayerW
#define peerUTMPlayer					peerUTMPlayerW
#define peerKickPlayer					peerKickPlayerW
#define peerGetPlayerPing				peerGetPlayerPingW
#define peerGetPlayersCrossPing			peerGetPlayersCrossPingW
#define peerPingPlayer					peerPingPlayerW
#define peerGetPlayerInfoNoWait			peerGetPlayerInfoNoWaitW
#define peerGetPlayerInfo				peerGetPlayerInfoW
#define peerIsPlayerHost				peerIsPlayerHostW
#define peerGetPlayerFlags				peerGetPlayerFlagsW
#define peerGetReady					peerGetReadyW
#define peerStartGame					peerStartGameW
#define peerSetGlobalKeys				peerSetGlobalKeysW
#define peerGetPlayerGlobalKeys			peerGetPlayerGlobalKeysW
#define peerGetRoomGlobalKeys			peerGetRoomGlobalKeysW
#define peerSetRoomKeys					peerSetRoomKeysW
#define peerGetRoomKeys					peerGetRoomKeysW
#define peerSetGlobalWatchKeys			peerSetGlobalWatchKeysW
#define peerSetRoomWatchKeys			peerSetRoomWatchKeysW
#define peerGetGlobalWatchKey			peerGetGlobalWatchKeyW
#define peerGetRoomWatchKey				peerGetRoomWatchKeyW
#define peerStartAutoMatch				peerStartAutoMatchW
#define peerStartAutoMatchWithSocket	peerStartAutoMatchWithSocketW
#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Enumerated Types  
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// PEER
// Summary
//		The primary Peer object. This is passed in as the first
//		argument to most Peer SDK calls.
typedef void * PEER;

///////////////////////////////////////////////////////////
// PEERBool
// Summary
//		Peer's version of the standard boolean.
typedef enum
{
	PEERFalse,	// False.
	PEERTrue	// True.
} PEERBool;

///////////////////////////////////////////////////////////
// RoomType
// Summary
//		Types of Peer chat rooms.
// Members/Constants
//		TitleRoom	The main room for a game.
//		GroupRoom	A room which is, in general, for a particular type 
//					of gameplay (team, deathmatch, etc.).
//		StagingRoom	A room where players meet before starting a game.
//		NumRooms	Number of room types.
typedef enum
{
	TitleRoom,		// The main room for a game.
	GroupRoom,		// A room which is, in general, for a particular type 
					// of gameplay (team, deathmatch, etc.).
	StagingRoom,	// A room where players meet before starting a game.
	NumRooms		// Number of room types.
} RoomType;

///////////////////////////////////////////////////////////
// MessageType
// Summary
//		Types of messages.
// See Also
//		peerMessagePlayer, peerMessageRoom, peerPlayerMessageCallback, 
//		peerRoomMessageCallback
typedef enum
{
	NormalMessage,	// A normal chat message.
	ActionMessage,	// A notification message.
	NoticeMessage	// An action message.
} MessageType;

///////////////////////////////////////////////////////////
// PEERJoinResult
// Summary
//		Possible results when attempting to join a room. Passed into 
//		peerJoinRoomCallback().
typedef enum
{
	PEERJoinSuccess,     // The room was joined.

	PEERFullRoom,        // The room is full.
	PEERInviteOnlyRoom,  // The room is invite only.
	PEERBannedFromRoom,  // The local user is banned from the room.
	PEERBadPassword,     // An incorrect password (or none) was given for a passworded room.

	PEERAlreadyInRoom,   // The local user is already in or entering a room of the same type.
	PEERNoTitleSet,      // Can't join a room if no title is set.
	PEERNoConnection,    // Can't join a room if there's no chat connection.
	PEERAutoMatching,    // The user can't join a staging room during an AutoMatch attempt.

	PEERJoinFailed       // Generic failure.
} PEERJoinResult;

///////////////////////////////////////////////////////////
// PEERAutoMatchStatus
// Summary
//		Possible status values passed to the peerAutoMatchStatusCallback(). 
//		If PEERFailed, the match failed. Otherwise, this is the current status 
//		of the AutoMatch attempt.
typedef enum
{
	PEERFailed,		// The AutoMatch attempt failed.

	PEERSearching,	// Searching for a match (active).
	PEERWaiting,	// Waiting for a match (passive).
	PEERStaging,	// In a staging room with at least one other player, 
					// possibly waiting for more.
	PEERReady,		// All players are in the staging room, the host will call peerStartGame to trigger 
					// PEERComplete status.
	PEERComplete	// The AutoMatch attempt is now complete. Commence Nat Negotiation, making sure to 
					// share cookies using peerUTMPlayer.
					// The player is still in the staging room.
} PEERAutoMatchStatus;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Callback Function Types  
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerConnectCallback
// Summary
//		Callback received from a peerConnect() call to indicate success of 
//		failure in connecting to the Chat server.
// Parameters
//		peer			: [in] Initialized peer object.
//		success			: [in] PEERTrue if connection was successful, 
//								PEERFalse if connection failed.
//		failureReason	: [in] int value with the failure reason.
//		param			: [in] Pointer to user data. This is optional and will 
//								be passed unmodified to the callback function.
// Remarks
//		The peerConnectCallback function will notify the application of a 
//		success or failure in connecting to the Chat server. The 
//		"failureReason" tell you why the connection failed.
//		<p>
//		The values for the failureReason argument are:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		PEER_DISCONNECTED
//			<emit \</dt\>>
//				<emit \<dd\>>
//			Unable to connect to the server, or disconnected during the attempt.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_NICK_ERROR
//			<emit \</dt\>>
//				<emit \<dd\>>
//			There was a nick error that was not handled.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_LOGIN_FAILED
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The login info passed to peerConnectLogin was invalid.
//				<emit \</dd\>>
//		<emit \</dl\>>
// <p>
// See Also
//		peerConnect, peerDisconnect
typedef void (* peerConnectCallback)
(
	PEER peer,			// The peer object.
	PEERBool success,	// PEERTrue if success, PEERFalse if failure.
	int failureReason,	// If failure, the reason for it (PEER_DISCONNECTED, etc.)
	void * param		// User-data.
);

///////////////////////////////////////////////////////////
// peerDisconnectedCallback
// Summary
//		Callback received when the local player has been disconnected 
//		from the Chat server.
// Parameters
//		peer	: [in] Initialized peer object.
//		reason	: [in] A string containing the reason for disconnection.
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerDisconnectedCallback() is received when the connection to 
//		the Chat server gets disconnected, either from a call to 
//		peerDisconnect(), or a lost connection, or being kicked by the 
//		Chat server. To connect again, use peerConnect(). After 
//		reconnecting, any Chat channel rooms the local client was in will 
//		need to be rejoined.
//		<p>
//		The reason argument string can be any of the following:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		Disconnected
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The SDK tells you that your connection to the chat server was lost.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Excess Flood
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The Chat server disconnected you because you were flooding the 
//			chat servers with too many messages.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		INVALID GAMENAME (LOGGED)
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The Chat server disconnected the client since the game name 
//			provided was wrong.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		KILLED - KLINE
//			<emit \</dt\>>
//				<emit \<dd\>>
//			If the user was previously banned, and the ban has not been 
//			removed, the following error message will be sent after 
//			disconnecting him.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Unauthorized client (LOGGED)
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client is attempting to login using an unencrypted 
//			connection.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Unauthorized client
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client was insecurely connected to the Chat server and 
//			was therefore disconnected because the Chat server was switched 
//			to only support encrypted connections.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Login Timeout
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client failed to connect in the period of time designated 
//			for logins which is currently 90 seconds.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		US Takeover (too slow)
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The Chat server is forcing clients to disconnect because it 
//			is overloaded or the server is migrating clients to another server.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Flow Control Overflow
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The Chat server has too many users or channels.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Duplicate CD Key Use Detected
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key was used again by either the same person or 
//			someone else. Primarily, it will be the same user reconnecting.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Ping Timeout
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client was sent a ping and it never game back.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		Duplicate Login
//			<emit \</dt\>>
//				<emit \<dd\>>
//			Another user has logged in with the same Presence and Messaging
//			(GP) profile.
//				<emit \</dd\>>
//		<emit \</dl\>>
// <p>
//		This routine must be handled and an error indication must be presented to 
//		the user.  Indicates that, for example, the connection to the chat server
//		is lost.
// See Also
//		peerConnect, peerDisconnect
typedef void (* peerDisconnectedCallback)
(
	PEER peer,					// The peer object.
	const gsi_char * reason,	// The reason for the disconnection.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerRoomMessageCallback
// Summary
//		Callback received when a chat message is broadcast to a Chat 
//		channel room that the local client is in with peerMessageRoom().
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player who sent the 
//							message.
//		message		: [in] The text of the message.
//		messageType	: [in] The type of message to send: NormalMessage or 
//							ActionMessage.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerRoomMessageCallback is received when a remote player 
//		broadcasts a chat message to a Chat channel room that the local 
//		client is in by calling peerMessageRoom().
// See Also
//		peerMessageRoom
typedef void (* peerRoomMessageCallback)
(
	PEER peer,					// The peer object.
	RoomType roomType,			// The type of room that the message was in.
	const gsi_char * nick,		// The nick of the player who sent the message.
	const gsi_char * message,	// The text of the message.
	MessageType messageType,	// The type of message.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerRoomUTMCallback
// Summary
//		Callback received when a hidden message (i.e. a UTM, which stands for 
//		"Under the Table Message") is broadcast to a Chat channel room that 
//		the local client is in with peerUTMRoom().
// Parameters
//		peer			: [in] Initialized peer object.
//		roomType		: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick			: [in] The chat nickname of the player who sent the UTM
//		command			: [in] The command type of the UTM (RQI, NFO, GML).
//		parameters		: [in] The payload or content of the UTM.
//		authenticated	: [in] PEERTrue if authenticated, PEERFalse otherwise.
//		param			: [in] Pointer to user data. This is optional and will 
//								be passed unmodified to the callback function.
// Remarks
//		The peerRoomUTMCallback is received when a remote client broadcasts
//		a hidden message (i.e. a UTM, which stands for "Under the Table 
//		Message") to a Chat channel room of the type specified by the roomType 
//		argument and that the local client is also in by calling peerUTMRoom().
//		<p>
//		The command argument is a short string identifying the type of the 
//		UTM (e.g., RQI, NFO, GML). You may safely ignore any UTM types not 
//		generated by your game, as internal Peer UTMs also will be received 
//		through this callback. The parameters argument is the content or 
//		payload of the UTM.
//		<p>
//		UTMs are generally used to pass around arbitrary data between clients. 
// See Also
//		peerUTMRoom
typedef void (* peerRoomUTMCallback)
(
	PEER peer,						// The peer object.
	RoomType roomType,				// The type of room that the UTM was in.
	const gsi_char * nick,			// The nick of the player who sent the UTM.
	const gsi_char * command,		// The command type of this UTM.
	const gsi_char * parameters,	// Any payload or content of this UTM.
	PEERBool authenticated,			// True if this has been authenticated by the server.
	void * param					// User-data.
);

///////////////////////////////////////////////////////////
// peerRoomNameChangedCallback
// Summary
//		Callback received when a Chat channel room name that the local 
//		client is in changes as a result of the host calling 
//		peerSetRoomName().
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerRoomNameChangedCallback is received when the name of the 
//		Chat channel room of the type specified by the roomType argument 
//		and that the local client is in changes as a result of the host
//		calling peerSetRoomName().
//		<p>
//		This callback only serves as a notice of the name change. To get 
//		the actual name call peerGetRoomName() once this callback returns.
// See Also
//		peerGetRoomName, peerSetRoomName
typedef void (* peerRoomNameChangedCallback)
(
	PEER peer,			// The peer object.
	RoomType roomType,  // The type of room that the name changed in.
	void * param		// User-data
);

///////////////////////////////////////////////////////////
// peerRoomModeChangedCallback
// Summary
//		Callback received when one of the modes of a Chat channel 
//		that the local client is in changes because the host or
//		operator of the channel called chatSetChannelMode().
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		mode		: [in] The new channel mode for this room.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerRoomModeChangedCallback is received when the host or
//		operator of a Chat channel room of the type specified by the 
//		roomType argument and that the local client is in changes one of 
//		the modes for that Chat channel room by calling 
//		chatSetChannelMode(). 
//		<p>
//		These modes include InviteOnly, Private, and Moderated.
//		<p>
//		See chat.h in the Chat SDK for more information on Chat channel 
//		modes.
// See Also
//		chatSetChannelMode
typedef void (* peerRoomModeChangedCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room that the name changed in.
	CHATChannelMode * mode, // The current mode for this room.
	void * param			// User-data
);

///////////////////////////////////////////////////////////
// peerPlayerMessageCallback
// Summary
//		Callback received when a private chat message is received from 
//		another player calling peerMessagePlayer().
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] The Chat nickname of the player that sent
//							the private message.
//		message		: [in] The text of the private message.
//		messageType	: [in] The type of message sent. Most commonly 
//							NormalMessage or ActionMessage.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerPlayerMessageCallback is received when a remote player 
//		sends the local player a private message by calling 
//		peerMessagePlayer(). The messageType can be one of NormalMessage 
//		or ActionMessage.
// See Also
//		peerMessagePlayer()
typedef void (* peerPlayerMessageCallback)
(
	PEER peer,					// The peer object.
	const gsi_char * nick,		// The nick of the player who sent the message.
	const gsi_char * message,	// The text of the message.
	MessageType messageType,	// The type of message.
	void * param				// User-data
);

///////////////////////////////////////////////////////////
// peerPlayerUTMCallback
// Summary
//		Callback received when a private hidden message (i.e. a UTM, which 
//		stands for "Under the Table Message") is received from another 
//		player calling peerUTMPlayer().
// Parameters
//		peer			: [in] Initialized peer object.
//		nick			: [in] The Chat nickname of the player who sent 
//								this UTM.
//		command			: [in] The command type of the UTM (RQI, NFO, GML).
//		parameters		: [in] The content or payload of the UTM.
//		authenticated	: [in] True if the UTM was authenticated by the Chat 
//								server.
//		param			: [in] Pointer to user data. This is optional and will 
//								be passed unmodified to the callback function.
// Remarks
//		The peerPlayerUTMCallback is received when a hidden message (i.e. a 
//		UTM, which stands for "Under the Table Message") is received from
//		a remote player calling peerUTMPlayer(). 
//		<p>
//		The command argument is a short string identifying the type of the 
//		UTM (e.g., RQI, NFO, GML). You may safely ignore any UTM types not 
//		generated by your game, as internal Peer UTMs also will be received 
//		through this callback. The parameters argument is the content or 
//		payload of the UTM.
//		<p>
//		UTMs are generally used to pass around arbitrary data between clients. 
// See Also
//		peerUTMPlayer
typedef void (* peerPlayerUTMCallback)
(
	PEER peer,					// The peer object.
	const gsi_char * nick,		// The nick of the player who sent the UTM.
	const gsi_char * command,	// The command type of this UTM.
	const gsi_char * parameters,// The content or payload of this UTM.
	PEERBool authenticated,		// True if this has been authenticated by the server.
	void * param				// User-data
);

///////////////////////////////////////////////////////////
// peerReadyChangedCallback
// Summary
//		Callback received when the readiness state of a player in the
//		same StagingRoom Chat channel as the local client changes as
//		a result of a peerSetReady() call.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] The Chat nickname of the player whose ready
//						flag value changed.
//		ready	: [in] PEERTrue if ready, PEERFalse if not.
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
// Remarks
//		The peerReadyChangedCallback is received whenever the readiness 
//		state of a player in the same StagingRoom Chat channel as the 
//		local client changes as a result of a peerSetReady() call.
// See Also
//		peerSetReady, peerAreAllReady, peerGetReady, peerStartGame
typedef void (* peerReadyChangedCallback)
(
	PEER peer,				// The peer object.
	const gsi_char * nick,  // The nick of the player who's ready state changed.
	PEERBool ready,			// The player's new ready state.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerGameStartedCallback
// Summary
//		Callback received when the host of a StagingRoom Chat channel
//		that the local client is in calls peerStartGame().
// Parameters
//		peer	: [in] Initialized peer object.
//		server	: [in] The SBServer object of the game host.
//		message	: [in] An optional message sent by the game host.
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
// Remarks
//		The peerGameStartedCallback is received when the host of a 
//		StagingRoom Chat channel that the local client is in calls 
//		peerStartGame() to signal that the game session is beginning.
//		<p>
//		When this callback is received the local client's "playing" flag
//		is turned on, just as if peerStartPlaying() had been called.
//		<p>
//		The server argument is Server Browsing SDK SBServer object that
//		can be used to query host keys for the game host or the public
//		and private IP addresses of the game host via SBServer...() calls 
//		from the Server Browsing SDK.
//		<p>
//		The message argument may optionally contain information sent 
//		from the game host to help the clients join the game session, 
//		such as the port to connect to the game host on or a password
//		required to join the game session.
//		<p>
//		This is a notice to the other players in the StagingRoom that the 
//		game is starting. The host does not receive this callback. 
// See Also
//		peerStartGame, peerStartPlaying, SBServerGetPublicAddress,
//		SBServerGetPublicInetAddress
typedef void (* peerGameStartedCallback)
(
	PEER peer,					// The peer object.
	SBServer server,			// A server object representing this host.
	const gsi_char * message,	// A message that was passed into peerStartGame().
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerPlayerJoinedCallback
// Summary
//		Callback received when a client joins a Chat channel room that
//		the local client is also in.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player that joined.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerPlayerJoinedCallback is received when a client joins a
//		a Chat channel room of the type specified by the roomType argument
//		that the local client is also in. This function can be used to 
//		receive player joins.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom,
//		peerJoinStagingRoomByChannel
typedef void (* peerPlayerJoinedCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room that the player joined.
	const gsi_char * nick,  // The nick of the player that joined.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerPlayerLeftCallback
// Summary
//		Callback received when a player in a Chat channel room 
//		that the local client is also in leaves that Chat channel
//		room.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player that left.
//		reason		: [in] An optional explanation string.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerPlayerLeftCallback is received when a player leaves a 
//		Chat channel room of the type specified by the roomType argument
//		that the local client is also in. There is an optional "reason" 
//		string that may be provided by the client that left. One cause  
//		of this callback is a remote client making a peerLeaveRoom() call.
// See Also
//		peerLeaveRoom
typedef void (* peerPlayerLeftCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room that the player left.
	const gsi_char * nick,  // The nick of the player that left.
	const gsi_char * reason,// The reason the player left.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerKickedCallback
// Summary
//		Callback received when the local client has been removed from a
//		Chat channel room.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player that kicked the
//							local client.
//		reason		: [in] An optional explanation string that gives a 
//							reason for being kicked.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerKickedCallback is used to notify the local player that they 
//		have been removed from a Chat channel room. As the local client has 
//		already been removed from the Chat channel room by the time this
//		callback arrives, there is no need to call peerLeaveRoom(). 
//		<p>
//		If the local client was manually removed from the Chat channel room, 
//		the client that removed him from that room can optionally specify a 
//		a reason string that will be received in the reason argument.
//		<p>
//		If the local client was removed by a Chat server event, one of the
//		following reason strings will be received:
//		<p>
//		<emit \<dl\>>
//			<emit \<dt\>>
//		"Channel Removed"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The channel was removed from the Chat Server.  Everyone is kicked 
//			out of the channel.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Channel Reset"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The channel was reset from the Chat server. Everyone is kicked out 
//			of the channel.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Oper privs reset"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client had his operator privileges reset, and was kicked out of 
//			the room for the change to take effect.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Oper privs revoked"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The client had his operator privileges revoked, and was kicked out 
//			of the room for the change to take effect.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"CS dropped (Chat Server Restarting)"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The Chat Server is being restarted.
//				<emit \</dd\>>
//		<emit \</dl\>>
// <p>
// See Also
//		peerKickPlayer
typedef void (* peerKickedCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room that the player was kicked from.
	const gsi_char * nick,  // The Chat nick of the player that did the kicking.
	const gsi_char * reason,// An optional reason for the kick.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerNewPlayerListCallback
// Summary
//		Callback received when the entire player list for the specified 
//		room needs to be updated.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		The peerNewPlayerListCallback is received after turning off quiet mode.
//		It serves as notice that the player list for this room has been updated. 
//		To get the new list, call peerEnumPlayers() after returning from this
//		callback.
// See Also
//		peerSetQuietMode, peerEnumPlayers
typedef void (* peerNewPlayerListCallback)
(
	PEER peer,			// The peer object.
	RoomType roomType,  // The type of room.
	void * param		// User-data
);

///////////////////////////////////////////////////////////
// peerChangeNickCallback
// Summary
//		Callback received when the local client attempts to change
//		his Chat nick, such as from a local peerChangeNick() call.
// Parameters
//		peer	: [in] Initialized peer object.
//		success	: [in] PEERTrue if successful, PEERFalse if failure.
//		oldNick	: [in] The local client's old Chat nickname.
//		newNick	: [in] The local client's new Chat nickname, may be 
//						unchanged if the nickname change attempt was
//						not successful.
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
// Remarks
//		If success is set to PEERTrue, the attempt succeeded and newNick is the 
//		local user's new nickname. If success is set to PEERFalse, the attempt 
//		failed and the local user's nick is still oldNick.
// See Also
//		peerChangeNick
typedef void (* peerChangeNickCallback)
(
	PEER peer,					// The peer object.
	PEERBool success,			// PEERTrue if success, PEERFalse if failure.
	const gsi_char * oldNick,	// The old nickname.
	const gsi_char * newNick,	// The new nickname.
	void * param				// User-data.
 );

///////////////////////////////////////////////////////////
// peerPlayerChangedNickCallback
// Summary
//		Callback received when a player in a Chat channel that the local 
//		client is also in changes his or her Chat nickname, such as from 
//		a remote peerChangeNick() call.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		oldNick		: [in] The player's old Chat nickname.
//		newNick		: [in] The player's new Chat nickname.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerPlayerChangedNickCallback is called for any changes a player 
//		makes to his/her nick in a specified room. If in multiple rooms with 
//		the same player, this callback will be called for each common room 
//		when that player changes his nick.
// See Also
//		peerChangeNick
typedef void (* peerPlayerChangedNickCallback)
(
	PEER peer,					// The peer object.
	RoomType roomType,			// The type of the room the nick changed was in.
	const gsi_char * oldNick,	// The player's old nick.
	const gsi_char * newNick,	// The player's new nick.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerPlayerInfoCallback
// Summary
//		Callback received from joining a Chat channel room or from a 
//		peerGetPlayerInfo() call.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player.
//		IP			: [in] The player's IP address in string form: 
//							"xxx.xxx.xxx.xxx".
//		profileID	: [in] The Profile id of the player.
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		After joining a room, the peerPlayerInfoCallback function will be 
//		called for each player in the room who is using Peer with his IP 
//		and profile ID. Then it will be called one more time with nick set 
//		to NULL. This info is immediately available for anyone who joins 
//		the room after the local player.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom,
//		peerJoinStagingRoomByChannel, peerGetPlayerInfo, 
//		peerGetPlayerInfoNoWait
typedef void (* peerPlayerInfoCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The room the info was gotten in.
	const gsi_char * nick,	// The nick of the player the info is for.
	unsigned int IP,		// The player's IP.
	int profileID,			// The player's profile ID.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerPlayerFlagsChangedCallback
// Summary
//		Callback received when a player's flags have changed in the room.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The Chat nickname of the player whose flags changed.
//		oldFlags	: [in] The old flags value.
//		newFlags	: [in] The new flags value.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerPlayerFlagsChangedCallback is called when a player's flags 
//		change in the room of the specified roomType. 
//		<p>
//		Changes in flag values can also represent changes in Chat privileges, 
//		for example a player may become an operator, leave a room, enter a 
//		room, or start a StagingRoom.
//		<p>
//		The "flags" integer is a bitmask that contains the following values:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		PEER_FLAG_STAGING
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is in a StagingRoom.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_READY
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is readied up for a game.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_PLAYING
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is playing a game.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_AWAY
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is away.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_HOST
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is the host of the room.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_OP
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player is an op (+o) in this room.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_VOICE
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The player has voice (+v) in this room.
//				<emit \</dd\>>
//		<emit \</dl\>>
// See Also
//		peerGetPlayerFlags
typedef void (* peerPlayerFlagsChangedCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The room the flags were changed in.
	const gsi_char * nick,	// The player whose flags have changed.
	int oldFlags,			// The player's old flags.
	int newFlags,			// The player's new flags.
	void * param			// User-data
);

///////////////////////////////////////////////////////////
// peerPingCallback
// Summary
//		Callback received when an updated ping time to a player 
//		who shares a Chat channel with the local client is 
//		received.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] The Chat nickname of the remote player
//		ping	: [in] The ping time to the remote player
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerPingCallback provides the latest ping time to a player
//		who shares a Chat channel with the local client.
//		<p>
//		This callback is provided for informational purposes. You can use 
//		it to update an on-screen display for players preparing to play 
//		a game session together or to determine if too many reported 
//		ping times exceed the requirements for players to play together.
//		<p>
//		Peer stores this ping time independently and makes the average
//		ping time to the remote player available via the 
//		peerGetPlayerPing() function.
// See Also
//		peerGetPlayerPing, peerGetPlayersCrossPing, peerCrossPingCallback
typedef void (* peerPingCallback)
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The other player's nick.
	int ping,				// The ping.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerCrossPingCallback
// Summary
//		Callback received when a new cross-ping time between two 
//		players who share a StagingRoom Chat channel with the local
//		client is reported.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick1		: [in] The Chat nick of the first player (reporter).
//		nick2		: [in] The Chat nick of the second player (reportee).
//		crossPing	: [in] The cross ping time between the two players.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		This callback receives the most recent ping time between the first 
//		player (nick1) and the second player (nick2), as reported by the 
//		first player.
//		<p>
//		This callback is provided for informational purposes. You can use 
//		it to update an on-screen display for players in a StagingRoom
//		preparing to play a game session together or to determine if
//		too many reported ping times exceed the requirements for those players
//		to play together.
//		<p>
//		Peer stores this ping time independently and makes the average
//		ping time between the two players available via the 
//		peerGetPlayersCrossPing() function. Peer does not distinguish
//		between ping times in which nick1 reports a time to nick2 and
//		ping times in which nick2 reports a time to nick1.
// See Also
//		peerGetPlayersCrossPing, peerPingCallback, peerGetPlayerPing
typedef void (* peerCrossPingCallback)
(
	PEER peer,				// The peer object.
	const gsi_char * nick1,	// The first player's nick.
	const gsi_char * nick2,	// The second player's nick.
	int crossPing,			// The cross-ping.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerGlobalKeyChangedCallback
// Summary
//		Callback received when a global watch key changes value.
// Parameters
//		peer	: [in] Initialized peer object
//		nick	: [in] The player's nickname
//		key		: [in] The name of this key
//		value	: [in] The value of this key
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerGlobalKeyChangedCallback is called for watch keys when a 
//		room is joined, for watch keys when another player joins, and for 
//		any newly set watch keys.
// See Also
//		peerSetGlobalWatchKeys, peerSetGlobalKeys
typedef void (* peerGlobalKeyChangedCallback)
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The player whose key changed.
	const gsi_char * key,	// The key.
	const gsi_char * value,	// The value.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerRoomKeyChangedCallback
// Summary
//		Callback received when a room watch key or a broadcast key changes value.
// Parameters
//		peer		: [in] Initialized peer object
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] The player's chat nickname whose keys changed
//		key			: [in] The key that has changed
//		value		: [in] The value that is associated with this key that 
//							has changed
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		The peerRoomKeyChangedCallback is called for watch keys when a room 
//		is joined, for watch keys when another player joins, for any newly 
//		set watch keys, and when a broadcast watch key is changed.
// See Also
//		peerSetRoomKeys
typedef void (* peerRoomKeyChangedCallback)
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The room the player is in.
	const gsi_char * nick,  // The player whose key changed.
	const gsi_char * key,	// The key.
	const gsi_char * value,	// The value.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerQRServerKeyCallback
// Summary
//		Callback called while you are hosting a game for you to report 
//		a game host key value, such as the name of the map being played.
// Parameters
//		peer	: [in] Initialized peer object
//		key		: [in] The value associated with this key will be returned
//		buffer	: [in] The QR2 buffer for holding the data
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerQRServerKeyCallback function is called while hosting a game 
//		to report values for server keys. For example usage see the PeerTest 
//		sample. For a detailed explanation of how server reporting works, 
//		see the Query & Reporting 2 SDK documentation.
//		<p>
//		If in staging, and either not playing or peerStartGame was called 
//		with PEER_REPORT_INFO set in the reporting options, then Peer will 
//		report the following keys: HOSTNAME_KEY, NUMPLAYERS_KEY, MAXPLAYERS_KEY, 
//		GAMEMODE_KEY (only if not playing), and PASSWORD_KEY, and the callback 
//		will not be called for these keys. 	Peer will also always report the 
//		GROUPID_KEY if in a group room. The application is responsible for 
//		reporting any other server keys and is also responsible for these 
//		keys when Peer does not report them. Any other keys the application 
//		reports must be registered with the peerQRKeyListCallback.
typedef void (* peerQRServerKeyCallback)
(
	PEER peer,				// The peer object.
	int key,				// The key for which to report information.
	qr2_buffer_t buffer,	// Fill in the information using this buffer.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerQRPlayerKeyCallback
// Summary
//		Callback called while you are hosting a game for you to report 
//		the values of your player keys.
// Parameters
//		peer	: [in] Initialized peer object
//		key		: [in] The key for reporting information
//		index	: [in] The array index of the player to report
//		buffer	: [in] The buffer for data
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerQRPlayerKeyCallback function is called while hosting a game 
//		to report values for player keys. For example usage see the PeerTest 
//		sample. For a detailed explanation of how server reporting works, 
//		see the Query & Reporting 2 SDK documentation.
//		<p>
//		If in staging, and either not playing or peerStartGame was called 
//		with PEER_REPORT_PLAYERS set in the reporting options, then Peer will 
//		report the PLAYER__KEY and PING__KEY keys, and the callback will not 
//		be called for these key. The application is responsible for reporting 
//		any other player keys and is also responsible for these keys when Peer 
//		does not report them. Any other keys the application reports must be 
//		registered with the peerQRKeyListCallback. The number of players is 
//		set with the peerQRCountCallback().
typedef void (* peerQRPlayerKeyCallback)
(
	PEER peer,				// The peer object.
	int key,				// The key for which to report information.
	int index,				// The index of the player for which to report info.
	qr2_buffer_t buffer,	// Fill in the information using this buffer.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerQRTeamKeyCallback
// Summary
//		Callback called while you are hosting a game for you to report 
//		the values of your team keys.
// Parameters
//		peer	: [in] Initialized peer object
//		key		: [in] The key that will be returned
//		index	: [in] The array index of the team requested
//		buffer	: [in] The QR2 buffer for data
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerQRTeamKeyCallback is called while hosting a game to report 
//		values for team keys. For example usage see the PeerTest sample.
//		For a detailed explanation of how server reporting works, see the 
//		Query & Reporting 2 SDK documentation.
//		<p>
//		Peer does not report any team keys. The application is responsible 
//		for reporting any team keys. Any keys the application reports must 
//		be registered with the peerQRKeyListCallback. The number of teams 
//		is set with the peerQRCountCallback.
typedef void (* peerQRTeamKeyCallback)
(
	PEER peer,				// The peer object.
	int key,				// The key for which to report information.
	int index,				// The index of the team for which to report info.
	qr2_buffer_t buffer,	// Fill in the information using this buffer.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerQRKeyListCallback
// Summary
//		Callback called while you are hosting a game for you to report the
//		list of game host keys you intend to provide.
// Parameters
//		peer		: [in] Initialized peer object
//		type		: [in] A type of QR2 keys.  One of key_server, 
//							key_player, key_team.
//		keyBuffer	: [in] The buffer to append the key to
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		The peerQRKeyListCallback function is called while hosting a game to 
//		list keys the application will report. For example usage see the 
//		PeerTest sample. For a detailed explanation of how server reporting 
//		works, see the Query & Reporting 2 SDK documentation.
//		<p>
//		Peer already registers certain keys, and these do not need to be 
//		registered by the application. Peer registers the following server 
//		keys: HOSTNAME_KEY, NUMPLAYERS_KEY, MAXPLAYERS_KEY, GAMEMODE_KEY, 
//		PASSWORD_KEY (if a password is set), and GROUPID_KEY (if in a group 
//		room). Peer also registers the player keys PLAYER__KEY and PING__KEY. 
//		Any other keys used by the application must be registered in this 
//		callback.
typedef void (* peerQRKeyListCallback)
(
	PEER peer,					// The peer object.
	qr2_key_type type,			// The type of keys being asked for (key_server, key_player, or key_team).
	qr2_keybuffer_t keyBuffer,  // Fill in the keys using this buffer.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerQRCountCallback
// Summary
//		Callback called while you are hosting a game for you to report 
//		the number of players or teams you are hosting.
// Parameters
//		peer	: [in] Initialized peer object
//		type	: [in] A type of QR2 keys. Can be either key_server, 
//						key_player, or key_team.
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Returns
//		
// Remarks
//		The peerQRCountCallback function is called while hosting a game to 
//		get the number of players or teams. For example usage see the 
//		PeerTest sample app. For a detailed explanation of how server 
//		reporting works, see the Query & Reporting 2 SDK documentation.
//		<p>
//		If in staging, and either not playing or peerStartGame was called 
//		with PEER_REPORT_PLAYERS set in the reporting options, then Peer 
//		will report the number of players, and this callback will not be 
//		called for a player count. Otherwise, the application must report 
//		the number of players in this callback, and it must always report 
//		the number of teams (or 0 if not using teams).
typedef int (* peerQRCountCallback)
(
	PEER peer,			// The peer object.
	qr2_key_type type,	// The type of count to return (key_player or key_team).
	void * param		// User-data.
);

///////////////////////////////////////////////////////////
// peerQRAddErrorCallback
// Summary
//		Callback called when you are hosting a game if there was an 
//		error getting your game host published to the list.
// Parameters
//		peer		: [in] Initialized peer object
//		error		: [in] A QR2 error code when reporting fails
//		errorString	: [in] The error in string form
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		The peerQRAddErrorCallback function is called when an error is flagged 
//		while reporting to the server. This callback is called while reporting 
//		if there is an error reporting the server. To see the possible error 
//		codes, look for the qr2_error_t enumeration in qr2.h.
//		<p>
//		This must be handled appropriately.  An error indication must be presented
//		to the user in the event that this function is called.
typedef void (* peerQRAddErrorCallback)
(
	PEER peer,				// The peer object.
	qr2_error_t error,		// The type of error.
	gsi_char * errorString,	// A text string containing the error.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerQRNatNegotiateCallback
// Summary
//		Callback called while you are hosting a game when a NAT-negotiation 
//		cookie is received from a client that would like to NAT negotiate
//		to connect with you.
// Parameters
//		peer	: [in] Initialized peer object
//		cookie	: [in] Cookie received.  Usually an integer value 
//						randomly generated by the sender.
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerQRNatNegotiationCallback function is called when a 
//		NAT-negotiation cookie is received. See the NAT-negotiation documentation for 
//      more information.
//      Note: when automatching the NAT Negotiation cookie should be shared via UTM messages 
//      (peerUTMPlayer/peerPlayerUTMCallback).

typedef void (* peerQRNatNegotiateCallback)
(
	PEER peer,		// The peer object.
	int cookie,		// A cookie sent from a potential client.
	void * param	// User-data.
);

///////////////////////////////////////////////////////////
// peerQRPublicAddressCallback
// Summary
//		Callback called when you are hosting a game to report your 
//		publicly visible IP address to you.
// Parameters
//		peer	: [in] Initialized peer object
//		ip		: [in] The IP address of the host in string form: 
//						"xxx.xxx.xxx.xxx"
//		port	: [in] The port number of the host
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerQRPublicAddressCallback function is called once a server
//		begins hosting to tell that server its publicly visible IP 
//		address and port. This wraps qr2_publicaddresscallback_t from 
//		the Query and Reporting 2 SDK.
typedef void (* peerQRPublicAddressCallback)
(
	PEER peer,			// The peer object.
	unsigned int ip,	// The public reporting IP
	unsigned short port,// The public reporting port
	void * param		// User-data.
);

///////////////////////////////////////////////////////////
// peerNickErrorCallback
// Summary
//		Callback received from a peerConnect() call when there is a problem
//		with the local client's proposed Chat nickname.
// Parameters
//		peer				: [in] Initialized peer object.
//		type				: [in] One of the predefined error types.
//		nick				: [in] The player's nickname passed to peerConnect or 
//									peerRegisterUniqueNick.
//		numSuggestedNicks	: [in] The number of suggested nicknames.
//		suggestedNicks		: [in] A list of suggested nicknames.
//		param				: [in] Pointer to user data. This is optional and will
//									be passed unmodified to the callback function.
// Remarks
//		If peerNickErrorCallback is called with type set to PEER_IN_USE or 
//		PEER_INVALID, there was an error with the nick that was passed to 
//		peerConnect() or peerRetryWithNick(). The connection attempt is put on 
//		hold until peerRetryWithNick() is called. peerRetryWithNick() does not 
//		need to be called immediately, but should be called within a reasonable 
//		amount of time, or the connection attempt may timeout. If the nick passed 
//		to peerRetryWithNick() fails, peerNickErrorCallback will be called again 
//		(and as many times as needed) until the server accepts a nick.
//		<p>
//		To stop attempting reconnects, call peerRetryWithNick() with a NULL or 
//		an empty string for the nickname, which will cause the connectCallback 
//		passed to peerConnect() to be called with success set to false.
//		<p>
//		If peerNickErrorCallback is called with type equal to 
//		PEER_UNIQUENICK_EXPIRED or PEER_NO_UNIQUENICK, then there was a 
//		problem with the uniquenick associated with the profile passed to 
//		peerConnectLogin(). The connection attempt is put on hold until 
//		peerRegisterUniqueNick() is called. peerRegisterUniqueNick() does 
//		not need to be called immediately - the connection will stay alive 
//		until peerRegisterUniqueNick() is called. If the uniquenick passed 
//		to peerRegisterUniqueNick() is invalid or already being used, then 
//		this callback will be called again with type set to 
//		PEER_INVALID_UNIQUENICK, and with suggestedNicks member filled with 
//		an array of suggested uniquenicks (based on the uniquenick passed 
//		to peerRegisterUniqueNick). In that case, peerRegisterUniqueNick() 
//		should be called again with a new uniquenick.
typedef void (* peerNickErrorCallback)
(
	PEER peer,							// The peer object.
	int type,							// The type of nick error (PEER_IN_USE, PEER_INVALID, etc.)
	const gsi_char * nick,				// The bad nick.
	int numSuggestedNicks,				// The number of suggested nicks.
	const gsi_char ** suggestedNicks,	// The array of nicks.
	void * param						// User-data.
);

///////////////////////////////////////////////////////////
// peerAuthenticateCDKeyCallback
// Summary
//		Called when an attempt to authenticate a CD Key via 
//		peerAuthenticateCDKey() is completed.
// Parameters
//		peer	: [in] Initialized peer object.
//		result	: [in] Indicates the result of the attempt
//		message	: [in] A text message representing the result
//		param	: [in] Pointer to user data. This is optional and will be 
//						passed unmodified to the callback function.
// Remarks
//		The peerAuthenticateCDKeyCallback is called when an attempt to 
//		authenticate a CD Key is completed. If the result has a value of 1, 
//		the CD Key was authenticated. Otherwise, the CD key was not 
//		authenticated.
//		<p>
//		The possible strings passed in the message argument are:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		"Unknown CD Key"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key does not exist, or is mistyped.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Authenticated"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key was successfully authenticated by the backend.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"CD Key Disabled"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			This CD Key was disabled due to either misuse or abuse.
//				<emit \</dd\>>
//		<emit \</dl\>>
// <p>
// See Also
//		peerAuthenticateCDKey
typedef void (* peerAuthenticateCDKeyCallback)
(
	PEER peer,					// The peer object.
	int result,					// 1 if authenticated, otherwise not authenticated.
	const gsi_char * message,	// A string representing the result.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerJoinRoomCallback
// Summary
//		Callback received from the following calls:  peerJoinTitleRoom(), 
//		peerJoinGroupRoom(), peerJoinStagingRoom(), 
//		peerJoinStagingRoomByChannel(), peerCreateStagingRoom(), 
//		peerCreateStagingRoomWithSocket().
// Parameters
//		peer		: [in] Initialized peer object
//		success		: [in] PEERTrue if successful, PEERFalse if failure
//		result		: [in] Indicates the result of the attempt
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerJoinRoomCallback is called when an attempt to join or create a 
//		room has finished. If successful, the local player is now in that room, 
//		and will be until he either leaves with peerLeaveRoom(), or is kicked 
//		with peerKickPlayer() (or otherwise), or the connection is disconnected.
//		<p>
//		If the value of success is PEERFalse, use the result argument value to 
//		check the reason for the failure.
//		<p>
//		If the value of result is PEERBadPassword the user can be prompted to 
//		enter a password, and then the join can be attempted again.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom, 
//		peerJoinStagingRoomByChannel, peerCreateStagingRoom, 
//		peerCreateStagingRoomWithSocket, peerLeaveRoom, 
//		peerKickPlayer
typedef void (* peerJoinRoomCallback)
(
	PEER peer,				// The peer object.
	PEERBool success,		// PEERTrue if success, PEERFalse if failure.
	PEERJoinResult result,	// The result of the attempt.
	RoomType roomType,		// The type of room joined/created.
	void * param			// User-data.
);

///////////////////////////////////////////////////////////
// peerListGroupRoomsCallback
// Summary
//		Callback received from a peerListGroupRooms() call.
// Parameters
//		peer		: [in] Initialized peer object.
//		success		: [in] PEERTrue if successful, PEERFalse if failure
//		groupID		: [in] The group ID of the current group room
//		server		: [in] A valid SBServer object associated with this 
//							group room
//		name		: [in] The name of the group room
//		numWaiting	: [in] The number number of players in the room
//		maxWaiting	: [in] The maximum number of players allowed in the 
//							room
//		numGames	: [in] The number of games in the room
//		numPlaying	: [in] The number of players already in a game for 
//							the room
//		param		: [in] Pointer to user data. This is optional and is 
//							passed unmodified to this callback function.
// Remarks
//		The peerListGroupRoomsCallback gets called once for each GroupRoom 
//		when listing GroupRooms. After this has been called for each 
//		GroupRoom, it will be called one more time with "groupID" set to 0.
//		<p>
//		The groupIDs received in this callback can be used with 
//		peerJoinGroupRoom() to join a GroupRoom Chat channel.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		Group ids and their associated Chat channels are not created 
//		automatically for new game titles.  
// See Also
//		peerListGroupRooms, peerJoinGroupRoom, peerStartListingGames,
//		peerListingGamesCallback
typedef void (* peerListGroupRoomsCallback)
(
	PEER peer,				// The peer object.
	PEERBool success,		// PEERTrue if success, PEERFalse if failure.
	int groupID,			// A unique ID for this group.
	SBServer server,		// The server object for this group room.
	const gsi_char * name,	// The group room's name.
	int numWaiting,			// The number of players in the room.
	int maxWaiting,			// The maximum number of players allowed in the room.
	int numGames,			// The number of games either staging or running in the group.
	int numPlaying,			// The total number of players in games in the group.
	void * param			// User-data.
 );

///////////////////////////////////////////////////////////
// peerListingGamesCallback
// Summary
//		Callback received from a peerStartListingGames() call.
// Parameters
//		peer		: [in] Initialized peer object.
//		success		: [in] PEERTrue if successful, PEERFalse if failure
//		name		: [in] The name of the game
//		server		: [in] The valid SBServer object a game is associated 
//							with
//		staging		: [in] PEERTrue if this host set its staging flag
//		msg			: [in] A code indicating the type of game host update 
//							being received
//		progress	: [in] The progress of the initial host listing
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerListingGamesCallback is called with updates on available 
//		game hosts in response to calling peerStartListingGames().
//		<p>
//		peerListingGamesCallback is used to maintain a list of running 
//		games and StagingRoom Chat channels for prospective games.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		Group ids and their associated Chat channels are not created 
//		automatically for new game titles.
//		<p>
//		The "server" argument uniquely identifies each prospective game 
//		host and can also be used in Server Browsing SDK calls to get 
//		additional information about that game host.
//		<p>
//		If the "staging" argument is true, the game represented by this 
//		call to the peerListingGamesCallback hasn't started yet and the 
//		players are still in the StagingRoom Chat channel. In this case,
//		you can use the peerJoinStagingRoom() function to add the local 
//		client to the StagingRoom Chat channel for that prospective game. 
//		<p>
//		If the "staging" argument is false, you can pass the received 
//		server object to SBServerGetPublicInetAddress() or 
//		SBServerGetPublicAddress() to get the game host's IP address and 
//		begin your game-specific logic for connecting to an in-progress 
//		game session on that host.
//		<p>
//		The "password" argument will be set to 1 for games that are 
//		passworded. This can be checked by calling 
//		SBServerGetIntValue(server, "password", 0).
//		<p>
//		The "progress" argument is an int that starts at a value of zero (0)
//		when the initial list of prospective game hosts begins and 
//		increases as new game hosts are received via PEER_ADD messages, 
//		finally reaching a value of 100 when the PEER_COMPLETE message
//		is received, which indicates that the initial list of prospective
//		game hosts is finished and new calls to the peerListingGamesCallback
//		will just update this list.
//		<p>
//		The "msg" argument is a code indicating the type of game host update 
//		being received in each invocation of the peerListingGamesCallback:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		PEER_CLEAR
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			Please remove every prospective game host previously received. 
//			This should be handled as if PEER_REMOVE were called for every 
//			prospective game host previously added with PEER_ADD.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_ADD
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			This is a new prospective game host.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_UPDATE
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			This prospective game host was previously added with PEER_ADD, but 
//			has subsequently changed state or keys.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_REMOVE
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			Please remove this game host from the list. The server object for 
//			this server should not be used after peerListingGamesCallback 
//			returns.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_COMPLETE
//			<emit \</dt\>> 
//				<emit \<dd\>>
//			The initial listing of prospective game hosts is finished and 
//			updates will be received if these change. When first starting to 
//			list games, an initial list of prospective game hosts is received, 
//			then updated as new game hosts are launched and old game hosts are 
//			updated or removed. The "progress" argument should now equal 100.
//				<emit \</dd\>>
//		<emit \</dl\>>
// See Also
//		peerStartListingGames, peerStopListingGames, SBServerGetPublicAddress, 
//		SBServerGetPublicInetAddress, SBServerGetIntValue
typedef void (* peerListingGamesCallback)
(
	PEER peer,				// The peer object.
	PEERBool success,		// PEERTrue if success, PEERFalse if failure.
	const gsi_char * name,	// The name of the game being listed.
	SBServer server,		// The server object for this game.
	PEERBool staging,		// If PEERTrue, this is a staging room and not a 
							// running game.
	int msg,				// The type of game host update this is:
							// PEER_CLEAR
							//		Clear the list. This has the same effect as if 
							//		this was called with PEER_REMOVE for every 
							//		server listed.
							// PEER_ADD
							//		This is a new server. Add it to the list.
							// PEER_UPDATE
							//		This server is already on the list, and its 
							//		been updated.
							// PEER_REMOVE
							//		Remove this server from the list. The server 
							//		object for this server should not be used 
							//		again after this callback returns.
	int progress,			// The percent of servers that have been added, 0-100.
	void * param			// User-data.
 );

///////////////////////////////////////////////////////////
// peerEnumPlayersCallback
// Summary
//		Callback received once for each player as a result of 
//		a peerEnumPlayers() call.
// Parameters
//		peer		: [in] Initialized peer object
//		success		: [in] PEERTrue if successful, or PEERFalse if failure
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom
//		index		: [in] The index of the current player being enumerated
//		nick		: [in] The Chat nickname of that player
//		flags		: [in] The Flags of that player
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		Called once for each player in a room being enumerated, and once when 
//		finished, with "index" set to -1 and "nick" set to NULL.
//		<p>
//		The index is not an identifier of any sort, its just a way to count 
//		the number of players that have been enumerated.
//		<p>
//		It is not persistent.
// See Also
//		peerEnumPlayers
typedef void (* peerEnumPlayersCallback)
(
	PEER peer,				// The peer object.
	PEERBool success,		// PEERTrue if success, PEERFalse if failure.
	RoomType roomType,		// The room whose players are being enumerated.
	int index,				// The index of the player (0 to (N - 1)).  -1 when finished.
	const gsi_char * nick,	// The nick of the player.
	int flags,				// This player's flags (see #define's above). 
	void * param			// User-data.
 );

///////////////////////////////////////////////////////////
// peerGetPlayerInfoCallback
// Summary
//		Callback received from a peerGetPlayerInfo() call.
// Parameters
//		peer		: [in] Initialized peer object
//		success		: [in] PEERTrue if successful, PEERFalse if failure
//		nick		: [in] The chat nickname of the player who was 
//							queried
//		IP			: [in] The player's IP address in string form: 
//							"xxx.xxx.xxx.xxx"
//		profileID	: [in] The player's profile ID
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerGetPlayerInfoCallback function will have the information 
//		requested by a peerGetPlayerInfo() call. Success will be indicated
//		by the value of the "success" argument. If the player did not exist, 
//		or the nick was invalid, or Peer was not connected, then the "success"
//		argument will be set to PEERFalse.
// See Also
//		peerGetPlayerInfo, peerGetPlayerInfoNoWait
typedef void (* peerGetPlayerInfoCallback)
(
 PEER peer,				// The peer object.
 PEERBool success,		// PEERTrue if success, PEERFalse if failure.
 const gsi_char * nick,	// The player's nick.
 unsigned int IP,		// The player's IP, in network byte order.
 int profileID,			// The player's profile ID.
 void * param			// User-data.
 );

///////////////////////////////////////////////////////////
// peerGetGlobalKeysCallback
// Summary
//		Callback received from a peerGetPlayerGlobalKeys() or 
//		peerGetRoomGlobalKeys() call.
// Parameters
//		peer	: [in] Initialized peer object.
//		success	: [in] PEERTrue if successful, PEERFalse if failure.
//		nick	: [in] The Chat nickname of the player who owns 
//						these keys.
//		num		: [in] The number of key/value pairs in the array.
//		keys	: [in] Array of key names whose values were retrieved.
//		values	: [in] Array of corresponding key values retrieved.
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
// Remarks
//		The peerGetGlobalKeysCallback is received in response to either 
//		peerGetPlayerGlobalKeys() or peerGetRoomGlobalKeys() and 
//		reports the requested player's global keys.
// See Also
//		peerGetPlayerGlobalKeys, peerGetRoomGlobalKeys
typedef void (* peerGetGlobalKeysCallback)
(
	PEER peer,					// The peer object.
	PEERBool success,			// If PEERFalse, unable to get the keys.
	const gsi_char * nick,		// The player who owns these keys.
	int num,					// The number of keys.
	const gsi_char ** keys,		// The keys got.
	const gsi_char ** values,	// The values for the keys.
	void * param				// User-data.
 );

///////////////////////////////////////////////////////////
// peerGetRoomKeysCallback
// Summary
//		Callback received from a peerGetRoomKeys() call.
// Parameters
//		peer		: [in] Initialized peer object
//		success		: [in] PEERTrue if successful, PEERFalse if failure
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom
//		nick		: [in] The player's nickname
//		num			: [in] The Number of key/value pairs in the array
//		keys		: [in] Array of key names whose values will be retrieved
//		values		: [in] Array of values to set
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
// Remarks
//		The peerGetRoomKeysCallback function is called when peer wants to obtain 
//		the room keys for a specified room. If nick is NULL, these are keys for 
//		the room. Otherwise, they are keys for a player in the room.<p>
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be changed to underscores.
// See Also
//		peerGetRoomKeys
typedef void (* peerGetRoomKeysCallback)
(
	PEER peer,				// The peer object.
	PEERBool success,		// If PEERFalse, unable to get the keys.
	RoomType roomType,		// The room the keys are in.
	const gsi_char * nick,  // The player the keys are for, or NULL for the room.
	int num,				// The number of keys.
	gsi_char ** keys,		// The keys.
	gsi_char ** values,		// The values for the keys.
	void * param			// User-data.
 );

///////////////////////////////////////////////////////////
// peerAutoMatchStatusCallback
// Summary
//		Callback received when the AutoMatch state has changed.
// Parameters
//		peer	: [in] Initialized peer object.
//		status	: [in] The current status.
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
// Remarks
//		The peerAutoMatchStatusCallback is received when the AutoMatch
//		state changes. For example, when a game is joined or the user begins 
//		hosting a game. Refer to the status descriptions below for more 
//		information.
// See Also
//		peerStartAutoMatch, peerStartAutoMatchWithSocket
//		peerStopAutoMatch, peerIsAutoMatching, peerGetAutoMatchStatus
typedef void (* peerAutoMatchStatusCallback)
(
	PEER peer,					// The peer object.
	PEERAutoMatchStatus status, // The current status.
	void * param				// User-data.
);

///////////////////////////////////////////////////////////
// peerAutoMatchRateCallback
// Summary
//		Callback received when rating a game host to find a best match.
// Parameters
//		peer	: [in] Initialized peer object
//		match	: [in] A possible game host match
//		param	: [in] Pointer to user data. This is optional and will 
//						be passed unmodified to the callback function.
//
// Remarks
//		The peerAutoMatchRateCallback function is called one or more times 
//		for each game found. This allows the application to assign a rating 
//		to each game, and is used to determine the best fit for the user.
//		The rating value should be calculated based on correlation between 
//		user preferred settings and actual game settings.
//		<p>
//		Peer uses the callback to ask the application how well a particular
//		match fits what the user is looking for. The application checks the 
//		match and then returns a rating for it. If the rating is <=0 the match 
//		is considered unsuitable and Peer will not attempt that match. If the 
//		rating is >0 this match is considered suitable. The higher the rating, 
//		up to a maximum of INT_MAX, the more suitable the match. The exact 
//		scheme used to generate a rating is entirely up to the application. The 
//		match ratings are used by Peer both to check if a particular match meets 
//		the user's wishes and to compare matches to each other. Peer will first 
//		try to match with the highest rated possible match, then the second 
//		highest, etc.
// See Also
//		peerStartAutoMatch, peerStartAutoMatchWithSocket
//		peerStopAutoMatch, peerIsAutoMatching, peerGetAutoMatchStatus
typedef int (* peerAutoMatchRateCallback)
(
	PEER peer,		// The peer object.
	SBServer match, // Potential match to rate.
	void * param	// User-data.
 );

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Structures
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// PEERCallbacks
// Summary
//		A structure that is passed to peerInitialize to define the 
//		specific callback functions that will be registered for each
//		of these callback function types supported in Peer. The value
//		of param will be passed as the last argument to these 
//		callback functions each time any of them is invoked.	
// See Also
//		peerInitialize
typedef struct PEERCallbacks
{
	peerDisconnectedCallback disconnected;				// Called when the chat connection has been 
														// disconnected by the server.
	peerRoomMessageCallback roomMessage;				// Called when a chat message has arrived in 
														// one of the rooms the developer is in.
	peerRoomUTMCallback roomUTM;						// Called when an under-the-table message has 
														// arrived in a room the developer is in.
	peerRoomNameChangedCallback roomNameChanged;		// Called when the name of a room the developer 
														// is in has changed.
	peerRoomModeChangedCallback roomModeChanged;		// Called when the mode changed in  a room the 
														// developer is in.
	peerPlayerMessageCallback playerMessage;			// Called when a private chat message from another 
														// player has been received.
	peerPlayerUTMCallback playerUTM;					// Called when an under-the-table message has 
														// arrived from another player.
	peerReadyChangedCallback readyChanged;				// Called when another player in the same staging 
														// room as the user, has changed his ready status.
	peerGameStartedCallback gameStarted;				// Called when the host in the staging room launches 
														// the game.
	peerPlayerJoinedCallback playerJoined;				// Called when a player has joined one of the 
														// rooms the local player has joined.
	peerPlayerLeftCallback playerLeft;					// Called when a player has left one of the rooms 
														// the local player has joined.
	peerKickedCallback kicked;							// Called when the local player has been kicked 
														// from a room.
	peerNewPlayerListCallback newPlayerList;			// Called when the entire player list has been updated.
	peerPlayerChangedNickCallback playerChangedNick;	// Called when player in one of the rooms changed his nick.
	peerPlayerInfoCallback playerInfo;					// Called for all players (who are using peer) in 
														// a room shortly after joining.
	peerPlayerFlagsChangedCallback playerFlagsChanged;	// for all players (who are using peer) in a room. 
														// Called when a player's flags have changed.
	peerPingCallback ping;
	peerCrossPingCallback crossPing;
	peerGlobalKeyChangedCallback globalKeyChanged;
	peerRoomKeyChangedCallback roomKeyChanged;
	peerQRServerKeyCallback qrServerKey;
	peerQRPlayerKeyCallback qrPlayerKey;
	peerQRTeamKeyCallback qrTeamKey;
	peerQRKeyListCallback qrKeyList;
	peerQRCountCallback qrCount;
	peerQRAddErrorCallback qrAddError;
	peerQRNatNegotiateCallback qrNatNegotiateCallback;
	peerQRPublicAddressCallback qrPublicAddressCallback;
	void * param;										// A pointer to data that will be passed into each 
														// of the callbacks when triggered.
} PEERCallbacks;

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerInitialize
// Summary
//		Initialize the Peer SDK.
// Parameters
//		callbacks		: [in] a structure of local callback functions to
//								to register with Peer
// Returns
//		Initialized Peer SDK.
// Remarks
//		The peerInitialize function creates and initializes a PEER object, 
//		which is valid until it is passed to peerShutdown. The PEER object
//		is the first argument to most Peer SDK function calls. After 
//		initialization, the next two steps are usually to set the title 
//		with peerSetTitle() and then connect with peerConnect(). It is 
//		valid to have multiple PEER objects allocated at any given time, 
//		however this is usually not needed.
//		<p>
//		NULL is returned in case of an error, otherwise a peer object is 
//		returned. If a peer object is returned, peerShutdown() must be 
//		called to cleanup the object when it it no longer needed.
// See Also
//		peerShutdown
PEER peerInitialize
(
	PEERCallbacks * callbacks  // Global callbacks.
);

///////////////////////////////////////////////////////////
// peerConnect
// Summary
//		Connect to the chat server.
// Parameters
//		peer				: [in] Initialized peer object.
//		nick				: [in] Chat nickname.
//		profileID			: [in] Profile ID of the local client, or 0.
//		nickErrorCallback	: [in] Callback function is called if the nickname 
//									is invalid or is already being used.
//		connectCallback		: [in] Callback function is called when the connect 
//									operation is completed.
//		param				: [in] Pointer to user data. This is optional and will 
//									be passed unmodified to the callback function.
//		blocking			: [in] When set to PEERTrue this function will not return 
//									until the operation has completed.
// Remarks
//		The peerConnect() function opens a connection to the Chat server. 
//		The connection can be ended at any time by calling peerDisconnect().
//		Calling peerShutdown() will also close the connection. If the 
//		connection gets disconnected for any other reason (such as 
//		intermediate router failure), the peerDisconnectedCallback() 
//		callback will be called.
//		<p>
//		peerConnect() will fail if the Peer object is already connected.
//		<p>
//		Once connected to the backend chat server, Peer is fully enabled, 
//		and the user can join rooms, create rooms, list games, message 
//		other players, etc. Typically at this point the user is put in 
//		the game's title room via peerJoinTitleRoom().
//		<p>
//		Note: This function can still block even if the 'blocking' argument 
//		is set to PEERFalse in situations where DNS response is slow, as 
//		it depends on a blocking gethostbyname() call.
// See Also
//		peerConnectLogin, peerConnectPreAuth, peerDisconnect, peerIsConnected
void peerConnect
(
	PEER peer,									// The peer object.
	const gsi_char * nick,						// The nick to connect with.
	int profileID,								// The profileID, or 0 if no profileID.
	peerNickErrorCallback nickErrorCallback,	// Called if nick error.
	peerConnectCallback connectCallback,		// Called on complete.
	void * param,								// User-data.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerConnectLogin
// Summary
//		Connects a peer object to the backend chat server and then logs in using 
//		an account from the GameSpy Presence and Messaging (GP) system. A title 
//		must be set with peerSetTitle() before this function is called.
// Parameters
//		peer				: [in] Initialized peer object.
//		namespaceID			: [in] Namespace id for this title, use 
//								GSI_NAMESPACE_GAMESPY_DEFAULT if uncertain.
//		email				: [in] Email address of the profile to login with.
//		profilenick			: [in] Nickname of the profile to login with.
//		uniquenick			: [in] Registered uniquenick of the profile to login with.
//		password			: [in] Password of the profile to login with.
//		nickErrorCallback	: [in] Callback function to be called if a nickname error 
//									occurs.
//		connectCallback		: [in] Callback function to be called when the operation 
//									completes
//		param				: [in] Pointer to user data. This is optional and will be 
//									passed unmodified to the callback function.
//		blocking			: [in] When set to PEERTrue this function will not return 
//									until the operation has completed.
// Remarks
//		The peerConnectLogin() function attempts to connect a peer object to the 
//		backend Chat server and then log in using an account from the GameSpy 
//		Presence and Messaging (GP) system. The connection can be ended at any time 
//		by calling peerDisconnect(). Calling peerShutdown() will also close the 
//		connection. If the connection gets disconnected for any other reason (such 
//		as intermediate router failure), the peerDisconnectedCallback() callback 
//		will be called.
//		<p>
//		This function will fail if the Peer object is already connected.
//		<p>
//		Once connected to the backend chat server, Peer is fully enabled, and the 
//		user can join rooms, create rooms, list games, message other players, etc. 
//		Typically at this point the user is put in the game's title room via 
//		peerJoinTitleRoom().
//		<p>
//		There are two ways of specifying the account information. One way is to 
//		specify a uniquenick and password combination. In this case, email and 
//		profilenick should be NULL, and namespaceID should be greater than 0. 
//		The other way is to specify an email, profilenick, and password. In this 
//		case, uniquenick should be NULL, and namespaceID should be set to GSI_NAMESPACE_GAMESPY_DEFAULT 
//		for the GameSpy default namespace. 
//		<p>
//		If you are using a custom namespace, specify its namespace ID.
//		<p>
//		In general, we recommend using the default namespace value specified 
//		by the GSI_NAMESPACE_GAMESPY_DEFAULT constant.
//		<p>
//		Note: This function can still block even if the 'blocking' argument 
//		is set to PEERFalse in situations where DNS response is slow, as 
//		it depends on a blocking gethostbyname() call.
// See Also
//		peerConnect, peerConnectPreAuth, peerDisconnect, peerIsConnected,
//		peerJoinTitleRoom
void peerConnectLogin
(
	PEER peer,								// The peer object.
	int namespaceID,						// The namespace id for this title, use 
											// GSI_NAMESPACE_GAMESPY_DEFAULT if uncertain.
	const gsi_char * email,					// The email to login with.
	const gsi_char * profilenick,			// The profile to login with.
	const gsi_char * uniquenick,			// The uniquenick to login with.
	const gsi_char * password,				// The password for the login account.
	peerNickErrorCallback nickErrorCallback,// Called if nick error.
	peerConnectCallback connectCallback,	// Called on complete.
	void * param,							// User-data.
	PEERBool blocking						// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerConnectPreAuth
// Summary
//		Connects a peer object to the backend chat server and then logs in using 
//		authentication information from a parter authentication system.
// Parameters
//		peer				: [in] Initialized peer object.
//		authtoken			: [in] Authtoken for this login.
//		partnerchallenge	: [in] Partner challenge for this login.
//		nickErrorCallback	: [in] Callback function to be called if a nick error 
//									occurs.
//		connectCallback		: [in] Callback function to be called when the operation 
//									completes.
//		param				: [in] Pointer to user data.  This is optional and will 
//									be passed unmodified to the callback function.
//		blocking			: [in] When set to PEERTrue this function will not return 
//									until the operation has completed.
// Remarks
//		The peerConnectPreAuth() function attempts to connect a peer object to the 
//		backend Chat server and then login using an account from a partner 
//		authentication system. The connection can be ended at any time by calling 
//		peerDisconnect(). Calling peerShutdown() will also close the connection.
//		If the connection gets disconnected for any other reason (such as 
//		intermediate router failure), the peerDisconnectedCallback() callback 
//		will be called.
//		<p>
//		This function will fail if the Peer object is already connected.
//		<p>
//		Once connected to the backend Chat server, Peer is fully enabled, and 
//		the user can join rooms, create rooms, list games, message other players, 
//		etc. Typically at this point the user is put in the game's title room 
//		via peerJoinTitleRoom(). 
//		<p>
//		A title must be set with peerSetTitle() before this function is called.
//		<p>
//		Note: This function can still block even if the 'blocking' argument 
//		is set to PEERFalse in situations where DNS response is slow, as 
//		it depends on a blocking gethostbyname() call.
// See Also
//		peerConnect, peerConnectLogin, peerDisconnect, peerIsConnected
void peerConnectPreAuth
(
	PEER peer,								// The peer object.
	const gsi_char * authtoken,				// The authtoken for this login.
	const gsi_char * partnerchallenge,		// The partner challenge for this login.
	peerNickErrorCallback nickErrorCallback,// Called if nick error.
	peerConnectCallback connectCallback,	// Called on complete.
	void * param,							// User-data.
	PEERBool blocking						// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerRetryWithNick
// Summary
//		Use in response to a peerNickErrorCallback. This function allows 
//		the local client to retry the connection attempt with a different 
//		chat nickname.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] Set to zero unless directed otherwise by GameSpy.
// Remarks
//		If the peerNickErrorCallback is called with type set to PEER_IN_USE or 
//		PEER_INVALID, there was an error with the nick that was passed to 
//		peerConnect() or peerRetryWithNick(). The connection attempt is put on 
//		hold until peerRetryWithNick() is called. peerRetryWithNick() does not 
//		need to be called immediately, but should be called within a reasonable 
//		amount of time, or the connection attempt may timeout. If the nick passed 
//		to peerRetryWithNick() fails, peerNickErrorCallback will be called again 
//		(and as many times as needed) until the server accepts a nick.
//		<p>
//		To stop attempting reconnects, call peerRetryWithNick() with a NULL or 
//		an empty string for the nickname, which will cause the connectCallback 
//		passed to peerConnect() to be called with success set to false.
// See Also
//		peerNickErrorCallback, peerConnect
void peerRetryWithNick
(
	PEER peer,				// The peer object.
	const gsi_char * nick	//
);

///////////////////////////////////////////////////////////
// peerRegisterUniqueNick
// Summary
//		Register a unique nick. Call in response to peerNickErrorCallback.
// Parameters
//		peer		: [in] Initialized peer object.
//		namespaceID	: [in] Namespace ID for this title, use 
//							GSI_NAMESPACE_GAMESPY_DEFAULT if uncertain.
//		uniquenick	: [in] Unique nickname to register.
//		cdkey		: [in] User's cdkey. Uniquenick will be attached to this key.
// Remarks
//		The peerRegisterUniqueNick() function takes and registers the 
//		specified uniquenick.
//		<p>
//		If peerNickErrorCallback is called with type equal to 
//		PEER_UNIQUENICK_EXPIRED or PEER_NO_UNIQUENICK, then there was a 
//		problem with the uniquenick associated with the profile passed to 
//		peerConnectLogin(). The connection attempt is put on hold until 
//		peerRegisterUniqueNick() is called. peerRegisterUniqueNick() does 
//		not need to be called immediately - the connection will stay alive 
//		until peerRegisterUniqueNick() is called. If the uniquenick passed 
//		to peerRegisterUniqueNick() is invalid or already being used, then 
//		this callback will be called again with type set to 
//		PEER_INVALID_UNIQUENICK, and with suggestedNicks member filled with 
//		an array of suggested uniquenicks (based on the uniquenick passed 
//		to peerRegisterUniqueNick). In that case, peerRegisterUniqueNick() 
//		should be called again with a new uniquenick.
//		<p>
//		If uniquenick passed to peerRegisterUniqueNick() is NULL or an empty 
//		string, then the connect attempt will be aborted.
//		<p>
//		The backend imposes certain requirements on a uniquenick before it 
//		is registered. For details on what is checked, see the GameSpy 
//		Presence and Messaging SDK overview documentation.
// See Also
//		peerNickErrorCallback
void peerRegisterUniqueNick
(
	PEER peer,						// The peer object.
	int namespaceID,				// A GameSpy namespace id, use 
									// GSI_NAMESPACE_GAMESPY_DEFAULT if uncertain.
	const gsi_char * uniquenick,	// A uniquenick to attempt to register.
	const gsi_char * cdkey			// Cdkey value to associate with this uniquenick.
);

///////////////////////////////////////////////////////////
// peerIsConnected
// Summary
//		Returns PEERTrue if connected to the Chat server.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns PEERTrue if connected to the GameSpy Chat server, 
//		PEERFalse otherwise.
// Remarks
//		The peerIsConnected function tests to check whether the peer 
//		object has connected to the chat server. It returns PEERFalse 
//		if the peer object is not connected.
// See Also
//		peerConnect, peerConnectLogin, peerConnectPreAuth, peerDisconnect
PEERBool peerIsConnected
(
	PEER peer	// The peer object.
);

///////////////////////////////////////////////////////////
// peerSetTitle
// Summary
//		Set the game information to be used by the Peer SDK.
// Parameters
//		peer			: [in] Initialized peer object.
//		title			: [in] Your gamename, assigned by GameSpy.
//		qrSecretKey		: [in] The game title secret key value, assigned by GameSpy.
//		sbName			: [in] Gamename for which to return game hosts from 
//								GameSpy Server Browsing; this is usually the 
//								same as the value used for title.
//		sbSecretKey		: [in] The game hosts' secret key value that corresponds
//								with the sbName, usually the same as qrSecretKey.
//		sbGameVersion	: [in] Game version for the local client.
//		sbMaxUpdates	: [in] The maximum number of server queries the SDK will 
//								send out at one time. Set this to 20 if uncertain.
//		natNegotiate	: [in] Set to PEERTrue if NAT negotiation is supported.
//		pingRooms		: [in] An array of 3 PEERBool values, one for each index 
//								of TitleRoom, GroupRoom, and StagingRoom respectively 
//								to indicate which room types to automatically ping
//								for response time to the host.
//		crossPingRooms	: [in] An array of 3 PEERBool values, one for each index 
//								of TitleRoom, GroupRoom, and StagingRoom respectively 
//								to indicate which room types to automatically ping
//								for response time to all the other clients.
// Returns
//		This function returns PEERFalse if an error occurs, otherwise PEERTrue.
// Remarks
//		The peerSetTitle function sets title information for the Peer SDK such as 
//		that used in server browsing and reporting. This should be called after 
//		peerInitialize and before any of the connection functions: peerConnect, 
//		peerConnectLogin, peerConnectPreAuth.
//		<p>
//		The sbMaxUpdates parameter should be at least 20.
// See Also
//		peerGetTitle, peerClearTitle, peerInitialize, peerConnect, peerConnectLogin, 
//		peerConnectPreAuth, peerStayInRoom
PEERBool peerSetTitle
(
	PEER peer,							// The peer object.
	const gsi_char * title,				// The title to make current (e.g., gmtest).
	const gsi_char * qrSecretKey,		// QR secret key.
	const gsi_char * sbName,			// SB gamename.
	const gsi_char * sbSecretKey,		// SB secret key.
	int sbGameVersion,					// The version of the game doing the browsing.
	int sbMaxUpdates,					// The maximum number of concurent updates.
										// 20 minimum for modems or slow networks, 
										// more for faster networks.
	PEERBool natNegotiate,				// PEERTrue if the title supports NAT-negotiation.
	PEERBool pingRooms[NumRooms],		// To do pings in a room type, set that index to PEERTrue.
	PEERBool crossPingRooms[NumRooms]	// To do cross-pings in a room type, set that index to PEERTrue.
);

///////////////////////////////////////////////////////////
// peerGetTitle
// Summary
//		Gets the currently set title for Peer.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		
// Remarks
//		The peerGetTitle function retreives the currently set title.
//		It returns NULL if there is no title set.
// See Also
//		peerSetTitle, peerClearTitle
const gsi_char * peerGetTitle
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerClearTitle
// Summary
//		Resets the Peer SDK. peerSetTitle must be called before new 
//		operations are made.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerClearTitle function resets the Peer SDK back to an initialized 
//		state. Until a title is set again with peerSetTitle(), the Peer SDK 
//		will be unable to join rooms, list games, etc. The local client will
//		be disconnected from the TitleRoom Chat room unless peerStayInRoom 
//		has been set appropriately.
// See Also
//		peerSetTitle, peerGetTitle, peerStayInRoom
void peerClearTitle
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerStayInRoom
// Summary
//		Allows the local client to remain in the game's TitleRoom after 
//		peerClearTitle() is called. peerStayInRoom is rarely used.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] Only TitleRoom is currently supported.
// Remarks
//		Calling this function signals Peer to stay in a room even if the 
//		title is cleared or changed with peerClearTitle() or peerSetTitle().
//		This will only be in effect until the next call to peerSetTitle().
//		Only TitleRoom is currently supported. The function has no effect 
//		if no title is set.
// See Also
//		peerClearTitle
void peerStayInRoom
(
	PEER peer,			// The peer object.
	RoomType roomType	// Only TitleRoom is currently supproted.
);

///////////////////////////////////////////////////////////
// peerDisconnect
// Summary
//		Disconnect from the Chat server. peerShutdown() must still 
//		be called.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerDisconnect function disconnects the local client from 
//		the Chat server. peerShutdown() must still be called to clean
//		up the PEER object and free internally allocated memory.
// See Also
//		peerConnect, peerConnectLogin, peerConnectPreAuth, peerDisconnect
//		peerIsConnected
void peerDisconnect
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerShutdown
// Summary
//		Destructs the Peer SDK.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerShutdown function is used to destruct the Peer SDK. 
//		This frees all internally allocated memory. The Peer SDK 
//		object should not be used after this call. To use the SDK 
//		again, call peerInitialize(). This call will also disconnect 
//		any outstanding connections.
// See Also
//		peerInitialize
void peerShutdown
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerThink
// Summary
//		Allow the Peer SDK to do internal processing and receive callbacks. 
//		Should be called as part of a main application loop.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		Allow the Peer SDK to do internal processing and receive callbacks. 
//		Registered callbacks for completed events will be invoked during 
//		this call. All network communications, callbacks and other events 
//		will happen only during this call. Not calling this function often
//		enough can negatively affect Peer SDK performance including message
//		delivery and reported ping times. As a consequence it should be 
//		called at least every ~10ms, typically, within the program's main 
//		loop.
void peerThink
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetChat
// Summary
//		Returns the chat sdk object.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		The Chat SDK object being used by the Peer SDK or NULL if Peer isn't 
//		connected.
// Remarks
//		The peerGetChat function returns the Chat SDK object being used by 
//		the Peer SDK since the Peer SDK wraps the Chat SDK.
//		<p>
//		An application can use this function to get a reference to the Chat 
//		object, which allows it to access lower level functionality in the 
//		Chat SDK, if needed. For example, this could be used to join a 
//		separate Chat channel that is managed outside the Peer SDK.
//		<p>
//		This function will fail if Peer is not yet connected to the Chat 
//		server. The Chat object returned by this function will become 
//		invalid if the Peer object is disconnected.
// See Also
//		peerConnect, peerConnectLogin, peerConnectPreAuth, peerDisconnect
//		peerIsConnected
CHAT peerGetChat
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetPublicIP
// Summary
//		Returns the local public IP address.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns the local public IP address.
// Remarks
//		The peerGetPublicIP function returns the local public IP address.
//		If called while not connected, will return 0. The IP this returns 
//		is the externally visible IP (e.g. for NATs).
// See Also
//		peerGetPrivateIP
unsigned int peerGetPublicIP
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetPrivateIP
// Summary
//		Returns the local private IP address.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns the local private IP address.
// Remarks
//		The peerGetPrivateIP function returns the local private IP address.
//		If called while not connected, or if there is no private address, 
//		will return 0. A private address is any local IP in a private IP 
//		range. The IP masks for these ranges (as specified in RFC 1918) are 
//		10.*, 172.16-31.*, and 192.168.*.
// See Also
//		peerGetPublicIP
unsigned int peerGetPrivateIP
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetUserID
// Summary
//		Returns the user id number of the local client. Only valid with 
//		peerConnectLogin() or peerConnectPreAuth().
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns the user id number of the local client.
// Remarks
//		The peerGetUserID function retreives the user id number for the local
//		client. This uniquely identifies a user account (email and password 
//		combination). See the Presence and Messaging (GP) SDK documentation for 
//		more details on users.
// See Also
//		peerConnectLogin, peerConnectPreAuth
int peerGetUserID
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetProfileID
// Summary
//		Returns the profile id number of the local client. Only valid with 
//		peerConnectLogin() or peerConnectPreAuth().
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		The profile id number of the local client.
// Remarks
//		The peerGetProfileID function returns the profile id of the local 
//		client. This uniquely identifies a profile (nick/email/password or 
//		uniquenick/password). See the Presence and Messaging (GP) SDK 
//		documentation for more details on profiles.
// See Also
//		peerConnectLogin, peerConnectPreAuth
int peerGetProfileID
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetNick
// Summary
//		Returns the chat nickname of the local client.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		The Chat nickname of the local user or NULL if not connected.
// Remarks
//		The peerGetNick function returns the chat nickname of the local client.
//		The peer object must be initialized and connected to a chat server.
//		Otherwise a NULL is returned if not connected.
// See Also
//		peerConnect, peerConnectLogin, peerConnectPreAuth, peerDisconnect
//		peerIsConnected
const gsi_char * peerGetNick
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerFixNick
// Summary
//		Repairs an illegal chat nickname. Removes illegal characters from a 
//		nickname as well as invalid character combinations.
// Parameters
//		newNick	: [out] Corrected nickname. May be the same as oldNick if no 
//						issues are detected. Must provide storage for one more 
//						character than oldNick.
//		oldNick	: [in]	The nickname to be corrected or verified. 
// Remarks
//		The peerFixNick function replaces illegal characters in the nickname 
//		with the underscore ("_") character. This function will also replace 
//		leading numbers and illegal whitespace combinations. Since oldNick
//		may be prefixed with at least one underscore when it is put into 
//		newNick, newNick should be able to hold at least one character more 
//		than oldNick.
// See Also
//		peerChangeNick, peerTranslateNick
void peerFixNick
(
	gsi_char * newNick,			// The corrected nickname, must be at least one 
								// character larger than oldNick.
	const gsi_char * oldNick	// The candidate nickname to check.
);

///////////////////////////////////////////////////////////
// peerTranslateNick
// Summary
//		Removes the namespace extension from a nickname. Use this when working 
//		with unique nicknames in a public chat room.
// Parameters
//		nick		: [in] The current nickname.
//		extension	: [in] The game extension, assigned by GameSpy. This will 
//							be removed from the nickname.
// Returns
//		Returns the nickname, stripped of the namespace identifier.
// Remarks
//		The peerTranslateNick function is used to remove a namespace extension 
//		from a nickname. Nicknames that are registered in a game's namespace 
//		will include an indentifying extension, such as "-gspy". This extension 
//		should not be displayed to the user, but should be stripped before 
//		display.
// See Also
//		peerFixNick, peerChangeNick
const gsi_char * peerTranslateNick
(
	gsi_char * nick,			// The nick to translate
	const gsi_char * extension	// The extension to be removed.
);

///////////////////////////////////////////////////////////
// peerChangeNick
// Summary
//		Change the chat nickname associated with the local client. This 
//		does not affect the user's profile nickname or unique nickname.
// Parameters
//		peer		: [in] Initialized peer object.
//		newNick		: [in] The nickname to assign to the local user.
//		callback	: [in] Callback function will be called when the 
//							operation completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not 
//							return until the operation has completed.
// Remarks
//		The peerChangeNick function may be used to change a user's nickname 
//		as it appears in Chat. 
//		<p>
//		This has no affect on GameSpy profile nicknames or unique nicknames 
//		such as those used in the Presence and Messaging (GP) SDK for online 
//		status and buddy lists. 
//		<p>
//		Only one instance of a Chat nickname may be in use on the Chat server
//		at a time. This function will fail if newNick is invalid or in use.
//		This function will also fail if Peer is not connected.
//		<p>
//		Calling this function will cause a peerChangeNickCallback to be 
//		received in the local client to indicate if the Chat nickname change 
//		attempt was successful. 
//		<p>
//		If the Chat nickname is successfully changed a 
//		peerPlayerChangedNickCallback will be received by all remote clients 
//		who implement the Peer SDK and share a Chat channel with the local 
//		client to notify them of the nickname change.
// See Also
//		peerFixNick, peerTranslateNick, peerChangeNickCallback, 
//		peerPlayerChangedNickCallback
void peerChangeNick
(
	PEER peer,							// The peer object.
	const gsi_char * newNick,			// The nickname to which to change.
	peerChangeNickCallback callback,	// Called when finished.
	void * param,						// Passed to the callback.
	PEERBool blocking					// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerSetQuietMode
// Summary
//		Sets the Peer SDK to quiet mode or disables quiet mode. See remarks.
// Parameters
//		peer	: [in] Initialized peer object.
//		quiet	: [in] Set to PEERTrue to enable quiet mode or PEERFalse 
//						to disable it.
// Remarks
//		The peerSetQuietMode function is used to toggle quiet mode. When in 
//		quiet mode the Peer SDK will not receive Chat or other messages. 
//		This allows the user to remain logged into Chat without disrupting 
//		gameplay with extraneous message traffic.
//		<p>
//		If quiet mode is enabled, the Chat server will not send this user 
//		channel messages or channel join/parts. This will last until quiet 
///		mode is disabled. If quiet mode is disabled after being enabled for
//		the peerNewPlayerListCallback() will be called for each Chat room 
//		the local user is in to update the current list of players.
//		<p>
//		This function has no effect if the title is not set and the peer 
//		object is not connected.
// See Also
//		peerNewPlayerListCallback
void peerSetQuietMode
(
	PEER peer,		// The peer object.
	PEERBool quiet	// If PEERTrue, enable quiet mode.
);

///////////////////////////////////////////////////////////
// peerSetAwayMode
// Summary
//		Set the away mode, as it appears in Chat.
// Parameters
//		peer	: [in] Initialized peer object.
//		reason	: [in] Reason for being away. Set to NULL or "" if not away.
// Remarks
//		The peerSetAwayMode function sets the Chat away mode for the local 
//		player. If the reason string is set to an empty string or NULL, away 
//		mode will but turned off. If reason is set to a valid string, away 
//		mode will be turned on. 
//		<p>
//		Once turned on, away mode will persist until turned off, or until 
//		the Peer connection is disconnected. 
//		<p>
//		To check if a player is away, check for PEER_FLAG_AWAY after calling 
//		peerGetPlayerFlags(). To get the reason string for a player's away 
//		state, check for that player's special "away" global key with 
//		peerGetPlayerGlobalKeys().
//		<p>
//		This function has no effect if peer object is not connected.
// See Also
//		peerGetPlayerFlags, peerGetPlayerGlobalKeys
void peerSetAwayMode
(
	PEER peer,				// The peer object.
	const gsi_char * reason	// The away reason. Set to NULL or "" to clear away status.
);

///////////////////////////////////////////////////////////
// peerParseQuery
// Summary
//		Pass a manually received server query to the Peer SDK. Use with 
//		peerStartReportingWithSocket() or peerCreateStagingRoomWithSocket().
// Parameters
//		peer	: [in] Initialized peer object.
//		query	: [in] String of query data received on the socket.
//		len		: [in] String length of query, not including the NULL.
//		sender	: [in] The address this query was received from.
// Remarks
//		The peerParseQuery function can be used to translate Peer packets into 
//		data. If hosting a room or a game using shared sockets, then this 
//		function must be used to pass data received on that socket to Peer.
//		<p>
//		When data is received on the socket, your application must determine 
//		if the data needs to be passed to Peer, or if it is data for your 
//		application. This can be done by checking if first two bytes in a 
//		packet are equal to the QR_MAGIC_1 and QR_MAGIC_2 constants.
//		<p>
//		Again, this function only needs to be called if reporting over a 
//		shared socket, which is initiated by either 
//		peerCreateStagingRoomWithSocket() or peerStartReportingWithSocket().
// See Also
//		peerCreateStagingRoomWithSocket, peerStartReportingWithSocket
void peerParseQuery
(
	PEER peer,					// The peer object.
	char * query,				// String of query data.
	int len,					// The length of the string, not including the NULL.
	struct sockaddr * sender	// The address the query was received from.
);

///////////////////////////////////////////////////////////
// peerAuthenticateCDKey
// Summary
//		Used to authenticate the local user's CD Key before putting that 
//		user in a Chat channel on the Chat server.
// Parameters
//		peer		: [in] Initialized peer object.
//		cdkey		: [in] The CD Key to validate. Presumably a valid CD 
//							Key for the currently set game title.
//		callback	: [in] Callback function will be called when the 
//							operation has completed.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not 
//							return until the operation has completed.
// Remarks
//		The peerAuthenticateCDKey function may be used to authenticate a 
//		user's CD Key before putting that user in a Chat channel on the 
//		Chat server. This should not be a substitute for a CD Key to 
//		authenticate actual gameplay. The results of the CD Key 
//		authentication check will be returned in peerAuthenticateCDKeyCallback.
// See Also
//		peerSetTitle, peerAuthenticateCDKeyCallback
void peerAuthenticateCDKey
(
	PEER peer,								// The peer object.
	const gsi_char * cdkey,					// The CD key to authenticate.
	peerAuthenticateCDKeyCallback callback,	// Called when finished.
	void * param,							// Passed to the callback.
	PEERBool blocking						// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerSendNatNegotiateCookie
// Summary
//		Send a NAT negotiation cookie to a server.
// Parameters
//		peer	: [in] Initialized peer object.
//		ip		: [in] IP address of the remote server.
//		port	: [in] Port of the remote server.
//		cookie	: [in] NN Cookie to send. This is an integer value 
//						generated from among the full possible int range 
//						by the sender.
// Remarks
//		The peerSendNatNegotiationCookie function sends a NAT negotiation cookie 
//		to the NAT negotiation server. Whatever method is used to generate the 
//		cookie should exercise the full possible range of int values, whether
//		produced by random generation or a hash function. 
//		<p>
//		peerSendNatNegotiateCookie should not be used with the AutoMatch feature. 
//		Instead, after an automatch cookies should be shared via UTM messages (peerUTMPlayer).
//      <p>
//      See the NAT negotiation documentation for further details on NAT negotiation cookies.
void peerSendNatNegotiateCookie
(
	PEER peer,				// The peer object.
	unsigned int ip,		// IP (network byte order) of the NAT server 
							// to send the cookie to.
	unsigned short port,	// Port (host byte order) of the NAT server 
							// to send the cookie to.
	int cookie				// The NN cookie to send.
);

//DOM-IGNORE-BEGIN 
///////////////////////////////////////////////////////////
// peerSendMessageToServer
// Summary
//		
// Parameters
//
void peerSendMessageToServer
(
	PEER peer,
	unsigned int ip,
	unsigned short port,
	const char * data,
	int len
);
//DOM-IGNORE-END

///////////////////////////////////////////////////////////
// peerAlwaysGetPlayerInfo
// Summary
//		Tell the Peer SDK to always retrieve IP and profile information 
//		for other room members.
// Parameters
//		peer	: [in] Initialized peer object.
//		always	: [in] Set to PEERTrue to have the SDK automatically retrieve 
//						player IP and profile information.
// Remarks
//		The peerAlwaysGetPlayerInfo function is used to tell the Peer SDK to 
//		automatically retrieve player IP and profile information each time a 
//		the local client joins a Chat room.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom
void peerAlwaysGetPlayerInfo
(
	PEER peer,			// The peer object.
	PEERBool always		// If true, always get player info.
);

///////////////////////////////////////////////////////////
// peerJoinTitleRoom
// Summary
//		Puts the local client in the TitleRoom Chat channel for the 
//		currently set game title.
// Parameters
//		peer		: [in] Initialized peer object.
//		password	: [in] Optional password of the title room. Usually 
//							NULL.
//		callback	: [in] Callback function to be called when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not 
//							return until the operation has completed.
// Remarks
//		The peerJoinTitleRoom function puts the local client in the TitleRoom 
//		Chat channel for the currently set game title. This function is 
//		typically called right after connecting, and it is only valid if a 
//		game title is set. If the user is already in a title room, this 
//		function will fail.
//		<p>
//		Any other clients in the TitleRoom Chat channel that implement the
//		Peer SDK will receive the peerPlayerJoinedCallback.
//		<p>
//		The Peer object must be connected to the Chat server and have a title 
//		set before joining any Chat rooms.
// See Also
//		peerJoinRoomCallback, peerSetTitle, peerGetTitle, peerStayInRoom,
//		peerClearTitle, peerLeaveRoom, peerPlayerJoinedCallback
void peerJoinTitleRoom
(
	PEER peer,									// The peer object.
	const gsi_char password[PEER_PASSWORD_LEN],	// An optional password, normally NULL.
	peerJoinRoomCallback callback,				// Called when finished.
	void * param,								// Passed to the callback.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerJoinGroupRoom
// Summary
//		Puts the local client in the specified GroupRoom Chat channel.
// Parameters
//		peer		: [in] Initialized peer object.
//		groupID		: [in] ID number of the group. See remarks.
//		callback	: [in] Callback function to be called when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not return 
//							until the operation has completed.
// Remarks
//		The peerJoinGroupRoom function puts the local client in the GroupRoom 
//		Chat channel specified by the groupID argument. 
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		Any other clients in the GroupRoom Chat channel that implement the
//		Peer SDK will receive the peerPlayerJoinedCallback.
//		<p>
//		The Peer object must be connected to the Chat server and have a title 
//		set before joining any Chat rooms.
// See Also
//		peerJoinRoomCallback, peerListGroupRooms, peerSetTitle, peerLeaveRoom,
//		peerGetGroupID, peerSetGroupID, peerStartListingGames,
//		peerListingGamesCallback, peerPlayerJoinedCallback
void peerJoinGroupRoom
(
	PEER peer,						// The peer object.
	int groupID,					// The ID for the GroupRoom to join.
	peerJoinRoomCallback callback,	// Callback called when finished.
	void * param,					// Passed to the callback.
	PEERBool blocking				// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerGetGroupID
// Summary
//		Returns the current GroupRoom Chat channel id set resulting from 
//		peerJoinGroupRoom() or peerSetGroupID().
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		The current GroupRoom Chat channel id or zero if Peer is not initialized
//		and connected to a GroupRoom Chat channel.
// Remarks
//		The peerGetGroupID function returns the current GroupRoom Chat channel id. 
//		This id is set automatically when joining a room with peerJoinGroupRoom() 
//		or when manually set using peerSetGroupID().
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles.
//		<p>
//		If the Peer object is not initialized or not connected to a GroupRoom Chat
//		channel, this function returns zero.
// See Also
//		peerJoinGroupRoom, peerSetGroupID, peerListGroupRooms, 
//		peerStartListingGames, peerListingGamesCallback
int peerGetGroupID
(
	PEER peer	// The peer object.
);

///////////////////////////////////////////////////////////
// peerSetGroupID
// Summary
//		Manually set the Group Id. Use this with caution as a local
//		client's Group Id should be set automatically by 
//		peerJoinGroupRoom().
// Parameters
//		peer	: [in] Initialized peer object.
//		groupID	: [in] Integer ID to set. 0 for no group.
// Remarks
//		The peerSetGroupID function sets the local client's Group Id 
//		manually. This function is not safe to call for automatically 
//		assigned GroupRoom Chat channel ids. Setting groupID to 0 is 
//		equivalent to leaving the GroupRoom Chat channel.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		This function is not needed if using peerJoinGroupRoom().
// See Also
//		peerGetGroupID, peerJoinGroupRoom, peerStartListingGames,
//		peerListingGamesCallback
void peerSetGroupID
(
	PEER peer,		// The peer object.
	int groupID		// The GroupRoom Chat channel id value to set
);

///////////////////////////////////////////////////////////
// peerJoinStagingRoom
// Summary
//		Puts the local client in a StagingRoom Chat channel specified 
//		by host server.
// Parameters
//		peer		: [in] Initialized peer object.
//		server		: [in] SBServer object.
//		password	: [in] Optional password for the staging room.
//		callback	: [in] Callback function to be called when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not return 
//							until the operation has completed.
// Remarks
//		This function puts the local client in a StagingRoom Chat channel 
//		specified by host server.
//		<p>
//		StagingRooms allow a group of players who are considering starting 
//		a game session together to chat for example to discuss the details 
//		for the game session (such as what map, rules, or settings will be 
//		used for the game) as the game session is prepared or before the 
//		game session is started.
//		<p>
//		Players in a StagingRoom can also see one another ping and crossping
//		times to determine if network latency is suitable for mutual game 
//		play.
//		<p>
//		The "server" argument is one of the server objects received by the 
//		peerListingGamesCallback.
//		<p>
//		The "password" argument must match the password set by the 
//		peerCreateStagingRoom() call that created this StagingRoom Chat 
//		channel. Passwords may not contain spaces. We recommend either 
//		scanning the password entered by the user for spaces and warning him 
//		if it contains any or stripping out spaces, as long as spaces in 
//		passwords are prevented in a consistent manner.
//		<p>
//		This function will only work if the server was listed with its 
//		"staging" argument set to true. Otherwise, the game session is 
//		considered already launched and should be joined directly.
//		<p>
//		If the server object was obtained in the peerListingGamesCallback that
//		matches the game title set by the most recent call to peerSetTitle(), 
//		then server listing should be stopped with peerStopListingGames() 
//		before calling peerJoinStagingRoom.
//		<p>
//		Any other clients in the StagingRoom Chat channel that implement the
//		Peer SDK will receive the peerPlayerJoinedCallback.
//		<p>
//		If no title is set, this function does nothing. 
// See Also
//		peerJoinRoomCallback, peerJoinStagingRoomByChannel, peerCreateStagingRoom
//		peerStartListingGames, peerListingGamesCallback, peerStopListingGames, 
//		peerLeaveRoom, peerPlayerJoinedCallback
void peerJoinStagingRoom
(
	PEER peer,									// The peer object.
	SBServer server,							// The server passed received in the 
												// peerlistingGamesCallback().
	const gsi_char password[PEER_PASSWORD_LEN],	// The password of the StagingRoom being joined. 
												// Can be NULL or "".
	peerJoinRoomCallback callback,				// Callback called when finished.
	void * param,								// Passed to the callback.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerJoinStagingRoomByChannel
// Summary
//		Puts the local client in a StagingRoom Chat channel specified 
//		by Chat channel name.
// Parameters
//		peer		: [in] Initialized peer object.
//		channel		: [in] Chat channel name.
//		password	: [in] Optional password of the room.
//		callback	: [in] Callback function to be called when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not 
//							return until the operation has completed.
// Remarks
//		This function puts the local client in a StagingRoom Chat channel 
//		specified by Chat channel name.
//		<p>
//		StagingRooms allow a group of players who are considering starting 
//		a game session together to chat for example to discuss the details 
//		for the game session (such as what map, rules, or settings will be 
//		used for the game) as the game session is prepared or before the 
//		game session is started.
//		<p>
//		Players in a StagingRoom can also see one another ping and crossping
//		times to determine if network latency is suitable for mutual game 
//		play.
//		<p>
//		The "channel" argument is a valid Chat channel name.
//		<p>
//		The "password" argument must match the password set by the 
//		peerCreateStagingRoom() call that created this StagingRoom Chat 
//		channel. Passwords may not contain spaces. We recommend either 
//		scanning the password entered by the user for spaces and warning him 
//		if it contains any or stripping out spaces, as long as spaces in 
//		passwords are prevented in a consistent manner.
//		<p>
//		This function will only work if the server was listed with its 
//		"staging" argument set to true. Otherwise, the game session is 
//		considered already launched and should be joined directly.
//		<p>
//		If the server object was obtained in the peerListingGamesCallback that
//		matches the game title set by the most recent call to peerSetTitle(), 
//		then server listing should be stopped with peerStopListingGames() 
//		before calling peerJoinStagingRoom.
//		<p>
//		Any other clients in the StagingRoom Chat channel that implement the
//		Peer SDK will receive the peerPlayerJoinedCallback.
//		<p>
//		If no title is set, this function does nothing. 
// See Also
//		peerJoinRoomCallback, peerJoinStagingRoom, peerCreateStagingRoom,
//		peerStartListingGames, peerListingGamesCallback, peerStopListingGames, 
//		peerLeaveRoom, peerPlayerJoinedCallback
void peerJoinStagingRoomByChannel
(
	PEER peer,									// The peer object.
	const gsi_char * channel,					// The channel to join.
	const gsi_char password[PEER_PASSWORD_LEN],	// The password of the room being joined. 
												// Can be NULL or "".
	peerJoinRoomCallback callback,				// Called when finished.
	void * param,								// Passed to the callback.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerCreateStagingRoom
// Summary
//		Creates a StagingRoom Chat channel in which the local client is 
//		the host if it doesn't exist, or rejoins the StagingRoom Chat 
//		channel previously created by the local client if he is no 
//		longer in this StagingRoom Chat channel.
// Parameters
//		peer		: [in] Initialized peer object.
//		name		: [in] The name of the StagingRoom.
//		maxPlayers	: [in] Maximum number of players allowed in the room.
//							Set to zero for unlimited.
//		password	: [in] Optional room password.
//							Set to NULL or "" for no password.
//		callback	: [in] Callback function to call when the 
//							operation completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue, this call will not return 
//							until the operation has completed.
// Remarks
//		The peerCreateStagingRoom function creates a StagingRoom Chat 
//		channel in which the local client is the host if it doesn't 
//		exist, or rejoins the StagingRoom Chat channel previously created 
//		by the local client if he is no longer in this StagingRoom Chat 
//		channel.
//		<p>
//		StagingRooms allow a group of players who are considering starting 
//		a game session together to chat for example to discuss the details 
//		for the game session (such as what map, rules, or settings will be 
//		used for the game) as the game session is prepared or before the 
//		game session is started.
//		<p>
//		Multiple simultaneous, independent StagingRooms may have the same 
//		name.
//		<p>
//		Setting the password argument to something other than NULL or "" 
//		will create a passworded StagingRoom Chat channel. Other players
//		who want to join a passworded StagingRoom Chat channel must pass in 
//		this case-sensitive password when they call peerJoinStagingRoom().
//		<p>
//		Spaces in passwords are not allowed. We recommend either scanning 
//		the password entered by the user for spaces and warning him if it 
//		contains any or stripping out spaces, as long as spaces in passwords 
//		are prevented in a consistent manner.
//		<p>
//		No more than maxPlayers players will be allowed in the StagingRoom, 
//		unless maxPlayers is set to 0, in which case no limit is set and 
//		the maxplayers key is not reported.
//		<p>
//		If the local client is in a GroupRoom Chat channel when this function 
//		is called, then the newly created StagingRoom will be reported as 
//		being a part of that group even if the local user leaves the first 
//		GroupRoom and joins a second GroupRoom.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		If peerCreateStagingRoom() is successful, the peerQR*Callback() 
//		callbacks will start getting called. These are used to provide 
//		information about this StagingRoom to other players. For more 
//		information on what to report in the callbacks, see the descriptions
//		of the peerQR*Callback() functions. 
//		<p>
//		If the user is already in a StagingRoom, this function returns 
//		PEERFalse and does nothing.
//		<p>
//		If no title is set, this function does nothing. 
// See Also
//		peerJoinRoomCallback, peerJoinStagingRoom, peerCreateStagingRoomWithSocket,
//		peerSetTitle, peerListGroupRooms, peerJoinGroupRoom, peerSetGroupId,
//		peerStartListingGames, peerListingGamesCallback, peerSetPassword,
//		peerGetRoomName, peerSetRoomName
void peerCreateStagingRoom
(
	PEER peer,									// The peer object.
	const gsi_char * name,						// The name of the room.
	int maxPlayers,								// The max number of players allowed in the room.
	const gsi_char password[PEER_PASSWORD_LEN],	// An optional password for the staging room
	peerJoinRoomCallback callback,				// Called when finished.
	void * param,								// Passed to the callback.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerCreateStagingRoomWithSocket
// Summary
//		Creates a StagingRoom Chat channel in which the local client is 
//		the host if it doesn't exist, or rejoins the StagingRoom Chat 
//		channel previously created by the local client if he is no 
//		longer in this StagingRoom Chat channel -- using the specified
//		externally-managed socket.
// Parameters
//		peer		: [in] Initialized peer object.
//		name		: [in] The name of the staging room.
//		maxPlayers	: [in] Maximum number of players allowed in the room.
//							Set to zero for unlimited.
//		password	: [in] Optional room password.
//							Set to NULL or "" for no password.
//		socket		: [in] The externally-managed socket to use to handle
//							queries.
//		port		: [in] The local port the externally-managed socket is bound 
//							to. 
//		callback	: [in] Callback function to call when the operation completes.
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue, this call will not return until 
//							the operation has completed.
// Remarks
//		The peerCreateStagingRoom function creates a StagingRoom Chat 
//		channel in which the local client is the host if it doesn't 
//		exist, or rejoins the StagingRoom Chat channel previously created 
//		by the local client if he is no longer in this StagingRoom Chat 
//		channel.
//		<p>
//		StagingRooms allow a group of players who are considering starting 
//		a game session together to chat for example to discuss the details 
//		for the game session (such as what map, rules, or settings will be 
//		used for the game) as the game session is prepared or before the 
//		game session is started.
//		<p>
//		Multiple simultaneous, independent StagingRooms may have the same 
//		name.
//		<p>
//		Setting the password argument to something other than NULL or "" 
//		will create a passworded StagingRoom Chat channel. Other players
//		who want to join a passworded StagingRoom Chat channel must pass in 
//		this case-sensitive password when they call peerJoinStagingRoom().
//		<p>
//		Spaces in passwords are not allowed. We recommend either scanning 
//		the password entered by the user for spaces and warning him if it 
//		contains any or stripping out spaces, as long as spaces in passwords 
//		are prevented in a consistent manner.
//		<p>
//		No more than maxPlayers players will be allowed in the StagingRoom, 
//		unless maxPlayers is set to 0, in which case no limit is set and 
//		the maxplayers key is not reported.
//		<p>
//		If the local client is in a GroupRoom Chat channel when this function 
//		is called, then the newly created StagingRoom will be reported as 
//		being a part of that group even if the local user leaves the first 
//		GroupRoom and joins a second GroupRoom.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		If peerCreateStagingRoom() is successful, the peerQR*Callback() 
//		callbacks will start getting called. These are used to provide 
//		information about this StagingRoom to other players. For more 
//		information on what to report in the callbacks, see the descriptions
//		of the peerQR*Callback() functions. 
//		<p>
//		If the user is already in a StagingRoom, this function does nothing.
//		<p>
//		If no title is set, this function does nothing. 
//		<p>
//		This function allows a game to share a socket with the Peer SDK, 
//		which can make hosting games behind a NAT proxy more viable.
//		<p>
//		The socket provided must be an previously created UDP socket. It will 
//		be used for sending out query replies, and any queries the application 
//		reads off of the socket must be passed to Peer using peerParseQuery(). 
//		<p>
//		This can be useful when running a game host behind a NAT/firewall/proxy.
//		For a full explanation of how this helps, see the "NAT and Firewall 
//		Support" Appendix of the "GameSpy Query and Reporting 2 SDK" overview
//		documentation.
//		<p>
//		If no title is set, this function does nothing. 
// See Also
//		peerJoinRoomCallback, peerJoinStagingRoom, peerCreateStagingRoom,
//		peerSetTitle, peerListGroupRooms, peerJoinGroupRoom, peerSetGroupId,
//		peerStartListingGames, peerListingGamesCallback, peerSetPassword,
//		peerGetRoomName, peerSetRoomName
void peerCreateStagingRoomWithSocket
(
	PEER peer,									// The peer object.
	const gsi_char * name,						// The name of the room.
	int maxPlayers,								// The max number of players allowed in the room.
	const gsi_char password[PEER_PASSWORD_LEN],	// An optional password for the staging room
	SOCKET socket,								// The socket to be used for reporting.
	unsigned short port,						// The local port to which the socket is bound.
	peerJoinRoomCallback callback,				// Called when finished.
	void * param,								// Passed to the callback.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerLeaveRoom
// Summary
//		Remove the local client from the current Chat channel room of 
//		the specified roomType.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		reason		: [in] Optional text reason broadcast to the room.
// Remarks
//		The peerLeaveRoom function will take the local player out of 
//		the current Chat channel room of the specified roomType.
//		Any room type can be left without affecting any other room.
//		<p>
//		Other clients in the same Chat channel room as the local client
//		that implement the Peer SDK will receive a peerPlayerLeftCallback.
//		<p>
//		This should be called by the host when he leaves the Staging Room, 
//		whether to go back out to the Title Room or to start the game 
//		(assuming there is no late entry or in-game use of the Staging 
//		Room chat channel). Otherwise the Staging Room will be reported
//		but not be joinable.  Likewise, peerLeaveRoom() should be called
//		to leave the title or group room when a client(or host) enters a 
//		Staging Room, assuming they are to leave the previous room's chat channel.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom, 
//		peerJoinStagingRoomByChannel, peerPlayerLeftCallback
void peerLeaveRoom
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room you want to leave:
							// TitleRoom, GroupRoom, or StagingRoom.
	const gsi_char * reason // The reason the player is leaving (can 
							// be NULL).
);

///////////////////////////////////////////////////////////
// peerListGroupRooms
// Summary
//		List all the GroupRoom Chat channels for the currently set title.
// Parameters
//		peer		: [in] Initialized peer object.
//		fields		: [in] Backslash delimited list of fields.
//		callback	: [in] Callback function to be called when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not return 
//							until the operation has completed.
// Remarks
//		The peerListGroupRooms functions lists all the GroupsRoom Chat channels 
//		for the currently set title. The peerListGroupRoomsCallback will be 
//		called once for each title, then it will be called once again with a 
//		"groupID" of 0. The groupIDs received in the callback can be used with 
//		peerJoinGroupRoom() to join a GroupRoom Chat channel.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		Group ids and their associated Chat channels are not created 
//		automatically for new game titles. 
//		<p>
//		If no title is set, this function does nothing. 
// See Also
//		peerListGroupRoomsCallback, peerJoinGroupRoom, peerStartListingGames
//		peerListingGamesCallback
void peerListGroupRooms
(
	PEER peer,								// The peer object.
	const gsi_char * fields,				// A backslash delimited list of fields.
	peerListGroupRoomsCallback callback,	// Called for each group room.
	void * param,							// Passed to the callback.
	PEERBool blocking						// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerStartListingGames
// Summary
//		Begin calling the peerListingGamesCallback to list the currently 
//		running game sessions and prospective game hosts that are 
//		advertising themselves via StagingRoom Chat channels.
// Parameters
//		peer		: [in] Initialized peer object.
//		fields		: [in] An array of registered QR2 keys to request from 
//							prospective game hosts.
//		numFields	: [in] The number of keys in the fields array.
//		filter		: [in] A SQL-like filter to limit the list of
//							prospective game hosts returned.
//		callback	: [in] Callback function to call with game host updates.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerStartListingGames function triggers Peer to start calling 
//		the peerListingGamesCallback with updates about the currently 
//		running game sessions and prospective game hosts that are 
//		advertising themselves via StagingRoom Chat channels.
//		<p>
//		This is typically used to maintain a list of prospective game hosts 
//		that the player chooses from. Once the player chooses a game host, 
//		he can be moved into the StagingRoom Chat channel for that game host 
//		by calling peerJoinStagingRoom() if that game is still "staging" 
//		(preparing to start a game session). 
//		<p>
//		Alternately, if the player chooses a game host that is not staging, 
//		but has already started a game session, the game host's IP address 
//		can be looked up by calling SBServerGetPublicInetAddress() or 
//		SBServerGetPublicAddress() and NAT negotiation or your game-specific 
//		logic for joining an in-progress game session can be initiated.
//		<p>
//		Once the local client has joined a game session, you are required to 
//		call peerStopListingGames() so that game host updates are not 
//		continuously sent to a client that no longer needs them.
//		<p>
//		The list of prospective games hosts returned to the 
//		peerListingGamesCallback after calling peerStartListingGames() is 
//		filtered by GroupRoom so that only prospective game hosts that 
//		came from the same GroupRoom via a peerJoinGroupRoom() call or 
//		that manually set the same Group Id via a peerSetGroupID() call 
//		as the local client will be returned.
//		<p>
//		If the local client isn't in a group, then only games and staging rooms 
//		that also aren't part of any group are listed.
//		<p>
//		GroupRooms are commonly used in this way to allow speakers of
//		a specific language to choose the GroupRoom Chat channel for their
//		language and then see only prospective game hosts that also passed
//		through that GroupRoom Chat channel and hence presumably speak the
//		same language.
//		<p>
//		A list of available Chat channel group ids for your title may be 
//		obtained by calling peerListGroupRooms(). Group ids and their associated
//		Chat channels are not created automatically for new game titles. 
//		<p>
//		After calling peerStartListingGames the peerListingGamesCallback will
//		first be called with one game host at a time to populate an initial 
//		list of prospective game hosts, then the peerListingGamesCallback will
//		be called periodically with updates that add hosts to, remove hosts from,
//		or indicate key or status changes among these game hosts. These updates
//		will continue to arrive periodically until peerStopListingGames() is 
//		called, or until the title is changed by a call to peerClearTitle() or 
//		peerSetTitle().
//		<p>
//		The filter argument is a SQL WHERE-clause-style boolean statement used 
//		to further restrict the list of prospective game hosts that will be 
//		returned to the peerListingGamesCallback, such as:
//		<p>
//		"gametype='ctf'" or <p>
//		"numplayers > 1 and numplayers < 8" or <p>
//		"hostname like '%[gsf]%'" or <p>
//		"(country = 'DE' or country = 'FR') and maxplayers >= 8".
//		<p>
//		The filter can be arbitrarily complex and supports standard SQL conditions 
//		and Boolean operations. 
//		<p>
//		The following fields can be filtered: hostport, gamever, location, hostname, 
//		mapname, gametype, gamemode, numplayers, maxplayers, groupid. 
//		<p>
//		This function will do nothing if there is no title set.
//		<p>
//		This function should only be called when the user starts looking at the game list.
//		Peer dynamically pushes list changes, so it is not necessary to stop and restart
//		the list manually.
// See Also
//		peerSetTitle, peerClearTitle, peerListGroupRooms, peerJoinGroupRoom, 
//		peerSetGroupID, peerListGamesCallback, peerJoinStagingRoom, 
//		peerStopListingGames, SBServerGetPublicInetAddress, 
//		SBServerGetPublicAddress
void peerStartListingGames
(
	PEER peer,							// The peer object.
	const unsigned char * fields,		// An array of registered QR2 keys to 
										// request from servers.
	int numFields,						// The number of keys in the fields array.
	const gsi_char * filter,			// A SQL-like filter on game hosts.
	peerListingGamesCallback callback,  // Called when finished.
	void * param						// Passed to the callback.
);

///////////////////////////////////////////////////////////
// peerStopListingGames
// Summary
//		Halts further game host list updates from being sent to 
//		the peerListingGamesCallback.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerStopListingGames function will stop a game host list update 
//		in progress. It will also cause the SDK to stop listening for game 
//		host state changes. The current server list is NOT cleared and remains 
//		accessible. Each game in this server list still is considered a valid 
//		game. The only time the games are invalidated or updated is if there 
//		is a call to peerStartListingGames() or the title is changed with a 
//		call to peerClearTitle() or peerSetTitle().
//		<p>
//		Once the local client has joined a game session, you are required to 
//		call peerStopListingGames() so that game host updates are not 
//		continuously sent to a client that no longer needs them.
//		<p>
//		Once a player hosts a game, peerStopListingGames() should be called, 
//		unless ongoing access to an updated list of currently active games is 
//		available without exiting the staging room (A possible implementation 
//		could be a tabbed UI that allows the user to toggle between title room/game 
//		list and a game-specific staging room).
// See Also
//		peerStartListingGames, peerListingGamesCallback, peerClearTitle,
//		peerSetTitle
void peerStopListingGames
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerUpdateGame
// Summary
//		Queries the game host specified by the server argument for 
//		updated game keys, such as numplayers, maxplayers, mapname, 
//		gametype, etc., as well as game-specific custom keys.
// Parameters
//		peer		: [in] Initialized peer object.
//		server		: [in] Game host to update.
//		fullUpdate	: [in] Set to PEERTrue to retrieve values for all 
//							keys, set to PEERFalse to receive only the
//							keys specified by peerStartListingGames().
// Remarks
//		The peerUpdateGame function is used to send a query to the game 
//		host specified by the server argument. This query will retrieve 
//		the game host key values from that game host. It is commonly used 
//		when a player has selected a specific game host from a list and
//		wants to see detailed settings for that game host.
//		<p>
//		The notification that this query is complete will be received in 
//		the peerListingGamesCallback as a PEER_UPDATE message, even if
//		game host updates are not being actively received from a 
//		peerStartListingGames() call. The server object received in the 
//		peerListingGamesCallback can then be queried for those keys using 
//		SBServerGet...() calls from the Server Browsing SDK.
//		<p>
//		This query will fall back to querying the GameSpy Master Server 
//		listing if the specified game host is not directly queryable.
//		<p>
//		If fullUpdate is set to PEERTrue, all of the keys will be 
//		requested.
//		<p>
//		If fullUpdate is set to PEERFalse, only the keys passed to
//		peerStartListingGames() as the fields argument will be requested.
//		<p>
//		If the game title is not set, this function does nothing.
//		<p>
//		This should only be called when a user selects a game.
//		Full updates for all games on the list cannot be requested automatically.
// See Also
//		peerListingGamesCallback, peerStartListingGames, peerSetTitle
void peerUpdateGame
(
	PEER peer,			// The peer object.
	SBServer server,	// The server object for the game to update.
	PEERBool fullUpdate	// If true, get all of the game's keys.
);


//DOM-IGNORE-BEGIN 
///////////////////////////////////////////////////////////
// peerUpdateGameByMaster
// Summary
//		Queries the GameSpy Master Server listing for updated game keys, 
//		such as numplayers, maxplayers, mapname, gametype, etc., as well 
//		as game-specific custom keys for the game host specified by the 
//		server argument. Please use peerUpdateGame() instead of this
//		function unless peerUpdateGame() is definitely not working for 
//		your needs.
// Parameters
//		peer		: [in] Initialized peer object
//		server		: [in] Game host to update
//		fullUpdate	: [in] Set to PEERTrue to retrieve values for all 
//							keys, set to PEERFalse to receive only the
//							keys specified by peerStartListingGames().
// Remarks
//		peerUpdateGameByMaster queries the GameSpy Master Server listing 
//		for updated game keys, such as numplayers, maxplayers, mapname, 
//		gametype, etc., as well as game-specific custom keys for the game 
//		host specified by the server argument. 
//		<p>
//		Please use peerUpdateGame() instead of this function unless 
//		peerUpdateGame() is definitely not working for your needs, since
//		peerUpdateGame() automatically falls back to querying the GameSpy
//		Master Server listing when the specified game host is not
//		directly queryable.
//		<p>
//		The notification that this query is complete will be received in 
//		the peerListingGamesCallback as a PEER_UPDATE message, even if
//		game host updates are not being actively received from a 
//		peerStartListingGames() call. The server object received in the 
//		peerListingGamesCallback can then be queried for those keys using 
//		SBServerGet...() calls from the Server Browsing SDK.
//		<p>
//		If fullUpdate is set to PEERTrue, all of the keys will be 
//		requested.
//		<p>
//		If fullUpdate is set to PEERFalse, only the keys passed to
//		peerStartListingGames() as the fields argument will be requested.
//		<p>
//		If the game title is not set, this function does nothing.
// See Also
//		peerUpdateGame, peerListingGamesCallback, peerStartListingGames, 
//		peerSetTitle
void peerUpdateGameByMaster
(
	PEER peer,			// The peer object.
	SBServer server,	// The server object for the game to update.
	PEERBool fullUpdate	// If true, get all of the game's keys.
);
//DOM-IGNORE-END

///////////////////////////////////////////////////////////
// peerUpdateGamePing
// Summary
//		Queries the game host specified by the server argument for 
//		an updated ping time from the local client to it.
// Parameters
//		peer		: [in] Initialized peer object
//		server		: [in] Game host to update ping time for
// Remarks
//		The peerUpdateGamePing function will send an ICMP echo to the 
//		game host specified by the server argument. The firewall or
//		NAT device between the specified game host and the internet
//		does not need to accept ICMP echo requests, in which case
//		the ping time to the nearest device that supports ICMP replies
//		will be returned.
//		<p>
//		The notification that this query is complete will be received in 
//		the peerListingGamesCallback as a PEER_UPDATE message, even if
//		game host updates are not being actively received from a 
//		peerStartListingGames() call. The server object received in the 
//		peerListingGamesCallback can then be queried for the updated
//		ping time via SBServerGetPing().
// See Also
//		peerUpdateGame, peerListingGamesCallback, peerStartListingGames,
//		SBServerGetPing
void peerUpdateGamePing
(
	PEER peer,			// The peer object.
	SBServer server		// The server object for the game to update.
);

///////////////////////////////////////////////////////////
// peerMessageRoom
// Summary
//		Send a chat message to all the clients in the same Chat 
//		channel as the local client.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		message		: [in] The text of the message to send.
//		messageType	: [in] The type of message to send, most commonly 
//							NormalMessage or ActionMessage.
// Remarks
//		The peerMessageRoom sends a chat message to all the clients 
//		in the same Chat channel as the local client. You specify 
//		which of a player's three possible Chat channel rooms 
//		(TitleRoom, GroupRoom, or StagingRoom) to send the message 
//		to with the roomType argument. A player can only be in one 
//		TitleRoom, one GroupRoom, and one StagingRoom at a time, 
//		so the roomType argument is sufficient to select which 
//		Chat channel room will receive the message.
//		<p>
//		All the players in the room, including the local player, will 
//		receive a peerRoomMessageCallback with the chat message.
//		<p>
//		This function will do nothing if the local client is not in a 
//		Chat room of the type specified by roomType. This function will
//		also do nothing if Peer is not connected or if the game title 
//		is not set.
// See Also
//		peerRoomMessageCallback, peerUTMRoom, peerUTMPlayer, peerSetTitle
void peerMessageRoom
(
	PEER peer,					// The peer object.
	RoomType roomType,			// The room type to send the message to.
	const gsi_char * message,	// The message.
	MessageType messageType		// The type of message.
);

///////////////////////////////////////////////////////////
// peerUTMRoom
// Summary
//		Send a hidden message (i.e. a UTM, which stands for "Under the 
//		Table Message") to all the clients in the same Chat channel as 
//		the local client.
// Parameters
//		peer			: [in] Initialized peer object.
//		roomType		: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		command			: [in] The command type of the UTM (RQI, NFO, GML).
//		parameters		: [in] The content or payload of the UTM.
//		authenticate	: [in] Set to PEERTrue to have server authenticate 
//								this UTM. Normally set this to PEERFalse.
// Remarks
//		The peerUTMRoom sends a hidden message (i.e. a UTM, which stands 
//		for "Under the Table Message") to all the clients in the same 
//		Chat channel as the local client. 
//		<p>
//		You specify which of a player's three possible Chat channel rooms 
//		(TitleRoom, GroupRoom, or StagingRoom) to send the UTM to with the 
//		roomType argument. A player can only be in one TitleRoom, one 
//		GroupRoom, and one StagingRoom at a time, so the roomType argument 
//		is sufficient to select which Chat channel room will receive the 
//		message.
//		<p>
//		All the players in the room, including the local player, will 
//		receive a peerRoomUTMCallback with the UTM.
//		<p>
//		UTMs are generally used to pass around arbitrary data between 
//		clients. The command argument is a short string identifying the 
//		type of the UTM (e.g., RQI, NFO, GML). The parameters argument is 
//		the content or payload of the UTM.
//		<p>
//		This function will do nothing if the local client is not in a 
//		Chat room of the type specified by roomType. This function will
//		also do nothing if Peer is not connected or if the game title 
//		is not set.
// See Also
//		peerRoomUTMCallback, peerUTMPlayer, peerMessageRoom, peerSetTitle
void peerUTMRoom
(
	PEER peer,						// The peer object.
	RoomType roomType,				// The room type to send the UTM to.
	const gsi_char * command,		// The type of the UTM.
	const gsi_char * parameters,	// The content or payload of the UTM.
	PEERBool authenticate			// If true, the server will authenticate 
									// this UTM (should normally be false).
);

///////////////////////////////////////////////////////////
// peerSetPassword
// Summary
//		Change or clear the password on the StagingRoom Chat channel
//		hosted by the local client.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] Must be set to StagingRoom for now.
//		password	: [in] Password to set on this room.
// Remarks
//		The peerSetPassword function changes or clears the password 
//		for the StagingRoom Chat channel hosted by the local client
//		from the password (or lack of one) that was set when 
//		CreateStagingRoom() was called.
//		<p>
//		If password is NULL or "", the password will be cleared.
//		<p>
//		Passwords may not contain spaces. We recommend either 
//		scanning the password entered by the user for spaces and warning 
//		him if it contains any or stripping out spaces, as long as spaces 
//		in passwords are prevented in a consistent manner.
//		<p>
//		This function will do nothing if the local client is not hosting
//		a StagingRoom Chat channel. This function will also do nothing 
//		if Peer is not connected or if the game title is not set.
// See Also
//		peerCreateStagingRoom, peerCreateStagingRoomWithSocket, 
//		peerSetTitle, peerSetRoomName, peerGetRoomName
void peerSetPassword
(
	PEER peer,									// The peer object.
	RoomType roomType,							// The room type, must be StagingRoom.
	const gsi_char password[PEER_PASSWORD_LEN]	// The new password.
);

///////////////////////////////////////////////////////////
// peerSetRoomName
// Summary
//		Change the name of the StagingRoom hosted by the local 
//		client.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] Must be set to StagingRoom for now.
//		name		: [in] The new name.
// Remarks
//		The peerSetRoomName function changes the name of the 
//		StagingRoom Chat channel hosted by the local client from 
//		the name that was set when CreateStagingRoom() was called.
//		<p>
//		This function will do nothing if the local client is not 
//		hosting a StagingRoom Chat channel. This function will also 
//		do nothing if Peer is not connected or if the game title is 
//		not set.
// See Also
//		peerGetRoomName, peerSetPassword, peerCreateStagingRoom, 
//		peerCreateStagingRoomWithSocket, peerSetTitle
void peerSetRoomName
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The room in which to set the name.
	const gsi_char * name	// The new name
);

///////////////////////////////////////////////////////////
// peerGetRoomName
// Summary
//		Returns the name of the TitleRoom, GroupRoom, or StagingRoom that 
//		the local client is in.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
// Returns
//		Returns the name of the TitleRoom, GroupRoom, or StagingRoom Chat
//		channel that the local client is in or NULL if the local client
//		is not in a Chat channel of the type specified by the roomType
//		argument.
// Remarks
//		Returns the name of the TitleRoom, GroupRoom, or StagingRoom 
//		Chat channel that the local client is in or NULL if the local 
//		client is not in a Chat channel of the type specified by the 
//		roomType argument.
//		<p>
//		This function will do nothing if Peer is not connected or if 
//		the game title is not set.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom, 
//		peerJoinStagingRoomByChannel, peerCreateStagingRoom, 
//		peerCreateStagingRoomWithSocket, peerSetRoomName, peerSetPassword,
//		peerGetRoomChannel
const gsi_char * peerGetRoomName
(
	PEER peer,			// The peer object.
	RoomType roomType	// The room to get the name for.
);

///////////////////////////////////////////////////////////
// peerGetRoomChannel
// Summary
//		Returns the name of the underlying Chat channel for the TitleRoom, 
//		GroupRoom, or StagingRoom that the local client is in.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
// Returns
//		Returns the name of the underlying Chat channel for the TitleRoom, 
//		GroupRoom, or StagingRoom that the local client is in or NULL if 
//		the local client is not in a Chat channel of the type specified 
//		by the roomType argument.
// Remarks
//		Returns the name of the underlying Chat channel for the TitleRoom, 
//		GroupRoom, or StagingRoom that the local client is in or NULL if 
//		the local client is not in a Chat channel of the type specified 
//		by the roomType argument.
//		<p>
//		This function will do nothing if Peer is not connected or if 
//		the game title is not set.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom, 
//		peerJoinStagingRoomByChannel, peerCreateStagingRoom, 
//		peerCreateStagingRoomWithSocket, peerSetRoomName, peerGetRoomName,
//		peerSetPassword
const gsi_char * peerGetRoomChannel
(
	PEER peer,			// The peer object.
	RoomType roomType	// The room to get the channel for.
);

///////////////////////////////////////////////////////////
// peerInRoom
// Summary
//		Determines if the local client is in a Chat channel of the 
//		specified roomType.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
// Returns
//		This function returns PEERTrue if the local client is in the 
//		room, PEERFalse otherwise.
// Remarks
//		The peerInRoom function checks whether or not the local player 
//		is in the specified room type. It returns PEERTrue if the local 
//		player is in the room specified.
// See Also
//		peerJoinTitleRoom, peerJoinGroupRoom, peerJoinStagingRoom, 
//		peerJoinStagingRoomByChannel, peerCreateStagingRoom, 
//		peerCreateStagingRoomWithSocket, peerGetRoomName,
//		peerGetRoomChannel
PEERBool peerInRoom
(
	PEER peer,			// The peer object.
	RoomType roomType	// The type of room to check for.
);

//DOM-IGNORE-BEGIN 
///////////////////////////////////////////////////////////
// peerSetTitleRoomChannel
// Summary
//		Set the underlying Chat channel to use as the TitleRoom. 
//		(This is rarely used, since the SDK sets TitleRoom Chat 
//		channel automatically.)
// Parameters
//		peer	: [in] Initialized peer object.
//		channel	: [in] Name of the channel to use as the title room.
// Remarks
//		The peerSetTitleRoomChannel function associates an underlying Chat 
//		channel to use as the TitleRoom. This function is normally not needed.
//		It must be called while a title is set, and will only last until a new 
//		title is set. If called with a NULL or empty channel, then Peer will 
//		determine the channel (the default behavior). If this is called while 
//		in a TitleRoom Chat channel, it won't take effect until the TitleRoom 
//		is left.
// See Also
//		peerJoinTitleRoom, peerJoinStagingRoomByChannel, peerGetRoomChannel
void peerSetTitleRoomChannel
(
	PEER peer,					// The peer object.
	const gsi_char * channel	// The channel to use for the title room.
);
//DOM-IGNORE-END

///////////////////////////////////////////////////////////
// peerGetHostServer
// Summary
//		Returns the SBServer object associated with the StagingRoom 
//		game host.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns the SBServer object associated with the StagingRoom game host 
//		or NULL if the local client is not in a StagingRoom Chat channel.
// Remarks
//		The peerGetHostServer function returns the SBServer object associated 
//		with the game host for the current StagingRoom that the local client is 
//		in, if any. Further data on the game host may be retrieved by passing the 
//		SBServer object into SBServer...() functions.
SBServer peerGetHostServer
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerSetStagingRoomMaxPlayers
// Summary
//		Changes the maximum number of players allowed in the StagingRoom
//		hosted by the local client.
// Parameters
//		peer		: [in] Initialized peer object.
//		maxPlayers	: [in] New maximum number of players.
// Remarks
//		The peerSetStagingRoomMaxPlayers function changes the maximum number 
//		of players allowed in the StagingRoom hosted by the local client from
//		the value set when the StagingRoom was created with 
//		peerCreateStagingRoom() or peerCreateStagingRoomWithSocket().
void peerSetStagingRoomMaxPlayers
(
	PEER peer,		// The peer object.
	int maxPlayers	// The new maximum number of players.
);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// Player Functions
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerEnumPlayers
// Summary
//		Enumerates through the players in a Chat room.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		callback	: [in] Callback function called when the operation
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
// Remarks
//		The peerEnumPlayers function will cause the peerEnumPlayersCallback
//		to be called once for each player in the Chat channel of the type
//		specified by roomType (TitleRoom, GroupRoom, StagingRoom).
//		<p>
//		After enumerating all the players, the peerEnumPlayersCallback will
//		be called once more with "index" set to -1 and "nick" set to NULL.
//		<p>
//		The enumeration is done using a local list of players, and the 
//		callbacks will be triggered from inside the peerEnumPlayers function 
//		call.
//		<p>
//		This only works if there is a title set, and the user is in a room
//		of the specified roomType.
// See Also
//		peerEnumPlayersCallback
void peerEnumPlayers
(
	PEER peer,							// The peer object.
	RoomType roomType,					// The room to enum the players in.
	peerEnumPlayersCallback callback,	// Called when finished.
	void * param						// Passed to callback.
);

///////////////////////////////////////////////////////////
// peerMessagePlayer
// Summary
//		Send a private chat message to the specified player.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] Chat nickname of the player to send the 
//							message to.
//		message		: [in] The message to send.
//		messageType	: [in] The type of message to send, commonly 
//							NormalMessage or ActionMessage.
// Remarks
//		The peerMessagePlayer function sends a private chat message to 
//		player specified by the nick argument.
//		<p>
//		As long as the nick is valid and matches up with a player, that 
//		player will receive the message in a peerPlayerMessageCallback.
//		<p>
//		This function only works while connected to the chat server. The 
//		client messaged must implement either the Peer or Chat SDK.
void peerMessagePlayer
(
	PEER peer,					// The peer object.
	const gsi_char * nick,		// The nick of the player to send the message to.
	const gsi_char * message,	// The message to send.
	MessageType messageType		// The type of message.
);

///////////////////////////////////////////////////////////
// peerUTMPlayer
// Summary
//		Send a private hidden message (i.e. a UTM, which stands for "Under 
//		the Table Message") to the client specified by the nick argument.
// Parameters
//		peer			: [in] Initialized peer object.
//		nick			: [in] Chat nickname of the target player.
//		command			: [in] The command type of the UTM (RQI, NFO, GML).
//		parameters		: [in] The content or payload of the UTM.
//		authenticate	: [in] Set to PEERTrue to have server authenticate 
//								this UTM. Normally set this to PEERFalse.
// Remarks
//		The peerUTMPlayer function sends a hidden message (i.e. a UTM, 
//		which stands for "Under the Table Message") to the client 
//		specified by the nick argument.
//		<p>
//		If the nick is valid and matches a client logged in to Chat, the 
//		specified client will receive a peerPlayerUTMCallback with the UTM.
//		<p>
//		UTMs are generally used to pass around arbitrary data between 
//		clients. The command argument is a short string identifying the 
//		type of the UTM (e.g., RQI, NFO, GML). The parameters argument 
//		is the content or payload of the UTM.
//		<p>
//		This function will do nothing if Peer is not connected or if the 
//		game title is not set.
// See Also
//		peerPlayerUTMCallback, peerUTMRoom, peerMessageRoom, peerSetTitle
void peerUTMPlayer
(
	PEER peer,						// The peer object.
	const gsi_char * nick,			// The nick of the player to send the UTM to.
	const gsi_char * command,		// The type of the UTM.
	const gsi_char * parameters,	// The content or payload of the UTM.
	PEERBool authenticate			// If true, the server will authenticate this 
									// UTM (should normally be false).
);

///////////////////////////////////////////////////////////
// peerKickPlayer
// Summary
//		Remove the player specified by the nick argument from a 
//		TitleRoom, GroupRoom, or StagingRoom Chat channel that
//		the local client is hosting or operating.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] Chat nickname of the client to remove.
//		reason		: [in] Optional explanation string sent to the 
//							target client.
// Remarks
//		The peerKickPlayer function removes the player specified by 
//		the nick argument from a TitleRoom, GroupRoom, or 
//		StagingRoom Chat channel that the local client is hosting or 
//		operating. The local client must have operator or host status
//		in order to kick other clients.
//		<p>
//		The reason explanation is optional.
//		<p>
//		The client to be removed must have the specified nick and must
//		be in a room of the specified type that is also hosted or 
//		operated by the local client.
//		<p>
//		If the client who was removed implements the Peer SDK, they 
//		will receive the peerKickedCallback.
// See Also
//		peerCreateStagingRoom, peerCreateStagingRoomWithSocket,
//		peerKickedCallback
void peerKickPlayer
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The room to kick the player from.
	const gsi_char * nick,	// The nick of the player to kick.
	const gsi_char * reason	// An optional reason for kicking the player
);

///////////////////////////////////////////////////////////
// peerGetPlayerPing
// Summary
//		Returns the cached ping time between the local player and the 
//		remote player specified by the nick argument.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] Nick of the player to check ping time to.
//		ping	: [out] This will be set to the cached ping time.
// Returns
//		This function return PEERTrue if a cached ping time was available 
//		or PEERFalse otherwise.
// Remarks
//		The peerGetPlayerPing function is used to retrieve the cached ping 
//		value for the specified remote player. Returns PEERFalse if we don't 
//		have or can't get a ping for this player. This function only works if 
//		a title is set.
// See Also
//		peerPingPlayer, peerGetPlayersCrossPing
PEERBool peerGetPlayerPing
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The player to get the ping for.
	int * ping				// The player's ping is stored here, if we have it.
);

///////////////////////////////////////////////////////////
// peerGetPlayersCrossPing
// Summary
//		Returns the average ping time between the two players specified 
//		by nick1 and nick2 who share a StagingRoom Chat channel with the 
//		local client.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick1		: [in] Chat nickname of player 1.
//		nick2		: [in] Chat nickname of player 2.
//		crossPing	: [out] This is set to the cross-ping time between
//							the two players, if available.
// Returns
//		This function returns PEERTrue if a cached cross ping time is 
//		available or PEERFalse otherwise.
// Remarks
//		The peerGetPlayersCrossPing function calculates the average 
//		ping time between the two players specified by nick1 and nick2 
//		who share a StagingRoom Chat channel with the local client.
//		<p>
//		This function returns PEERFalse if the ping time between the 
//		two specified players is not available. 
//		<p>
//		The order of the nicks specified as nick1 and nick2 does not
//		matter. That is, reversing nick1 and nick2 will return the
//		same result.
//		<p>
//		This function only works if a title is set and Peer is 
//		connected to the Chat server.
// See Also
//		peerGetPlayerPing, peerPingPlayer, peerCrossPingCallback
PEERBool peerGetPlayersCrossPing
(
	PEER peer,				// The peer object.
	const gsi_char * nick1,	// The first player.
	const gsi_char * nick2,	// The second player.
	int * crossPing			// The cross-ping is output here, if we have it.
);

///////////////////////////////////////////////////////////
// peerPingPlayer
// Summary
//		Send a ping request to a remote player specified by the
//		nick argument.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] Nickname of the remote player to ping.
// Returns
//		Returns PEERTrue if a ping attempt is made or PEERFalse otherwise.
// Remarks
//		The peerPingPlayer function sends a UDP ping to the specified player. 
//		Peer already automatically pings all other players in rooms of the 
//		types specified in the pingRooms argument given in the peerSetTitle() 
//		call.
//		<p>
//		peerPingPlayer does a one-time ping of the remote player specified by 
//		the nick argument in a non-ping room. However, pings must be enabled 
//		in at least one room for this to work, otherwise Peer does not 
//		initialize the UDP ping socket.
//		<p>
//		Also, peerAlwaysGetPlayerInfo() must be enabled so that Peer has IPs 
//		for other players in non-ping rooms.
// See Also
//		peerSetTitle, peerGetPlayerPing, peerGetPlayersCrossPing, 
//		peerAlwaysGetPlayerInfo
PEERBool peerPingPlayer
(
	PEER peer,				// The peer object.
	const gsi_char * nick	// The player to ping.
);

///////////////////////////////////////////////////////////
// peerGetPlayerInfoNoWait
// Summary
//		Retrieve the cached IP address and profile id of the player 
//		specified by the nick argument.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] Chat nickname of the target player.
//		IP			: [out] IP address of the target player.
//		profileID	: [out] Profile id of the target player.
// Returns
//		Returns PEERTrue if a cached copy was available or PEERFalse 
//		otherwise.
// Remarks
//		The peerGetPlayerInfoNoWait function returns the cached copy 
//		of the players IP and profile ID. It returns PEERFalse if this 
//		info is not available. This information may not be available 
//		if the local player is not in the same room as the specified 
//		player, or if the specified player's client does not implement 
//		the Peer SDK, or if the local client just joined a room and 
//		hasn't yet finished querying everyone's info.
//		<p>
//		Also, peerAlwaysGetPlayerInfo() must be enabled so that Peer 
//		queries these details.
// See Also
//		peerGetPlayerInfoCallback, peerGetPlayerInfo, peerAlwaysGetPlayerInfo
PEERBool peerGetPlayerInfoNoWait
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The nick of the player to get info about.
	unsigned int * IP,		// The player's IP will be output here.
	int * profileID			// The player's profile id will be output here.
);

///////////////////////////////////////////////////////////
// peerGetPlayerInfo
// Summary
//		Retrieve the IP address and profile id of the player specified 
//		by the nick argument from the Chat server.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] Chat nickname of the target player.
//		callback	: [in] Callback function called when the operation
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue, this call will not return 
//							until the operation has completed.
// Remarks
//		The peerGetPlayerInfo function queries the Chat server for the local 
//		player's IP and profile ID. The information will be available once 
//		the function is successful and the callback gets called. The callback 
//		will have both the profile id and IP address of the local player.
// See Also
//		peerGetPlayerInfoCallback, peerGetPlayerInfoNoWait
void peerGetPlayerInfo
(
	PEER peer,							// The peer object.
	const gsi_char * nick,				// The nick of the player to get info about.
	peerGetPlayerInfoCallback callback,	// The callback called with the results.
	void * param,						// Passed to callback.
	PEERBool blocking					// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerIsPlayerHost
// Summary
//		Returns PEERTrue if the player specified by the nick argument 
//		is the host or operator of the TitleRoom, GroupRoom, or 
//		StagingRoom Chat channel that the local client is also in.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] Nickname of the target player.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
// Returns
//		This function returns PEERTrue if the player specified by the nick 
//		argument is the host or operator of the TitleRoom, GroupRoom, or 
//		StagingRoom Chat channel that the local client is also in or 
//		PEERFalse otherwise.
// Remarks
//		This function returns PEERTrue if the player specified by the nick 
//		argument is the host or operator of the TitleRoom, GroupRoom, or 
//		StagingRoom Chat channel that the local client is also in or 
//		PEERFalse otherwise.
//		<p>
//		This is a convenience function for getting the value of the 
//		PEER_FLAG_HOST and PEER_FLAG_OP flags for the specified player
//		from peerGetPlayerFlags(). 
//		<p>
//		This function does nothing if no game title is set or Peer is 
//		not connected or if the local client is not in a room of the 
//		type specified by the roomType argument.
// See Also
//		peerGetPlayerFlags, peerPlayerFlagsChangedCallback, peerSetTitle
PEERBool peerIsPlayerHost
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The nick of the player to query.
	RoomType roomType		// The type of room to check.
);

///////////////////////////////////////////////////////////
// peerGetPlayerFlags
// Summary
//		Returns the cached player flag values for the player specified 
//		by the nick argument if that player is in the same TitleRoom, 
//		GroupRoom, or StagingRoom Chat channel as the local client.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] The chat nickname of the player to query.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		flags		: [out] An integer bitmask with the flags for the 
//							specified player.
// Returns
//		This function returns PEERTrue for success or PEERFalse 
//		otherwise.
// Remarks
//		Returns the cached player flag values for the player specified 
//		by the nick argument if that player is in the same TitleRoom, 
//		GroupRoom, or StagingRoom Chat channel as the local client.
//		<p>
//		These flags are cached from the broadcast key named "b_flags". 
//		Since this is cached, a Chat server query is not sent. 
//		<p>
//		Player flags are specific to a particular room, so the same 
//		player can have different player flags in different Chat rooms 
//		at the same time.
//		<p>
//		Flags might not be available yet for a player that just joined, 
//		or if the local client just joined the room. Also, flags might 
//		not be available for players that aren't using the Peer SDK. 
//		peerGetPlayerFlags will not return PEERFalse in that case. 
//		Instead, it will just set the flags to empty.
//		<p>
//		This function does nothing if no game title is set or Peer is 
//		not connected or if the local client is not in a room of the 
//		type specified by the roomType argument.
//		<p>
//		Each player flag is represented by one bit in the "flags" 
//		integer bitmask. These bitmask values are:
//		<emit \<dl\>>
//			<emit \<dt\>>
//		PEER_FLAG_STAGING
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is in a staging room.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_READY
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is ready to begin a game session.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_PLAYING
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is currently playing a game.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_AWAY
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is away.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_HOST
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is the host of the Chat channel room. 
//			The host of a Chat channel can remove other members.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_OP: 
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player is an operator (op, +o) in this Chat 
//			channel room. The operator of a Chat channel can 
//			remove other members.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		PEER_FLAG_VOICE
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The player has voice (+v) in this Chat channel room. 
//			Having "voice" means other members of this Chat channel
//			will see the voiced player's chat messages.
//				<emit \</dd\>>
//		<emit \</dl\>>
// See Also
//		peerIsPlayerHost, peerPlayerFlagsChangedCallback, peerSetTitle
PEERBool peerGetPlayerFlags
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The nick of the player to query.
	RoomType roomType,		// The type of room to check.
	int * flags				// The integer bitmask of player flags 
							// will be output here.
);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// Game Functions
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerSetReady
// Summary
//		Set the local client's flag that indicates whether he is 
//		ready to begin a game session.
// Parameters
//		peer	: [in] Initialized peer object.
//		ready	: [in] Set to PEERTrue to enable ready status.
// Remarks
//		The peerSetReady function sets the local client's flag that 
//		indicates whether he is ready to begin a game session.
//		Players default to "not ready" (PEERFalse).
//		<p>
//		This can be used by players in a StagingRoom Chat channel
//		to signal their readiness status to the prospective game 
//		host. Note that it is up to your game implementation to 
//		decide whether and how to handle or ignore the value of 
//		this flag for the various players in a StagingRoom Chat
//		channel.
//		<p>
//		Changes to this flag will be received in the 
//		peerReadyChangedCallback. An individual player's readiness
//		state can be queried with peerGetReady().
//		<p>
//		It is recommended that a StagingRoom game host not launch 
//		the game session with peerStartGame() until all clients 
//		show ready in peerAreAllReady().
// See Also
//		peerGetReady, peerReadyChangedCallback, peerStartListingGames
//		peerListingGamesCallback, peerJoinStagingRoom, peerAreAllReady,
//		peerStartGame
void peerSetReady
(
	PEER peer,		// The peer object.
	PEERBool ready	// Ready or not?
);

///////////////////////////////////////////////////////////
// peerGetReady
// Summary
//		Get the value of the flag that indicates readiness to begin a 
//		game session for the player with the specified nick who is 
//		in the same StagingRoom Chat channel as the local client.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] Nickname of the target player.
//		ready	: [out] Set to PEERTrue if the player is ready, 
//						PEERFalse if the player is not.
// Returns
//		This function returns PEERTrue if the ready state for the 
//		specified player could be found or PEERFalse otherwise.
// Remarks
//		The peerGetReady function gets the value of the flag that 
//		indicates readiness to begin a game session for the player 
//		with the specified nick who is in the same StagingRoom Chat 
//		channel as the local client.
//		<p>
//		This can be used by players in a StagingRoom Chat channel
//		to signal their readiness status to the prospective game 
//		host. Note that it is up to your game implementation to 
//		decide whether and how to handle or ignore the value of 
//		this flag for the various players in a StagingRoom Chat
//		channel.
//		<p>
//		Changes to this flag will be received in the 
//		peerReadyChangedCallback.
//		<p>
//		It is recommended that a StagingRoom game host not launch 
//		the game session with peerStartGame() until all clients 
//		show ready in peerAreAllReady().
//		<p>
//		This function does nothing if no game title is set or Peer 
//		is not connected or if the local client is not in a 
//		StagingRoom Chat channel.
// See Also
//		peerSetReady, peerReadyChangedCallback, peerSetTitle, 
//		peerStartListingGames, peerListingGamesCallback, 
//		peerJoinStagingRoom, peerAreAllReady, peerStartGame
PEERBool peerGetReady
(
	PEER peer,				// The peer object.
	const gsi_char * nick,	// The nick of the player to query.
	PEERBool * ready		// The queried player's ready state 
							// is output here.
);

///////////////////////////////////////////////////////////
// peerAreAllReady
// Summary
//		Get the aggregated value of the flags that indicate readiness 
//		to begin a game session for all players in the same StagingRoom 
//		Chat channel as the local client.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns PEERTrue if all players are ready or PEERFalse 
//		otherwise.
// Remarks
//		The peerAreAllReady function gets the aggregated value of 
//		the flags that indicate readiness to begin a game session 
//		for all players in the same StagingRoom Chat channel as 
//		the local client.
//		<p>
//		Changes to individual player's ready flags are received in 
//		the peerReadyChangedCallback. An individual player's readiness
//		state can be queried with peerGetReady().
//		<p>
//		It is recommended that a StagingRoom game host not launch 
//		the game session with peerStartGame() until all clients 
//		show ready in peerAreAllReady().
//		<p>
//		This function does nothing if no game title is set or Peer 
//		is not connected or if the local client is not in a 
//		StagingRoom Chat channel.
// See Also
//		peerGetReady, peerSetReady, peerReadyChangedCallback, 
//		peerSetTitle, peerStartListingGames, peerListingGamesCallback, 
//		peerJoinStagingRoom, peerStartGame
PEERBool peerAreAllReady
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerStartGame
// Summary
//		Called by the host of a StagingRoom Chat channel to signal 
//		that the game session is starting for the players in that
//		StagingRoom.
// Parameters
//		peer				: [in] Initialized peer object.
//		message				: [in] Optional message to send to each client.
//		reportingOptions	: [in] Bitfield flags used to set reporting 
//								options. (e.g. PEER_KEEP_REPORTING)
// Remarks
//		The peerStartGame function is called by the host of a StagingRoom 
//		Chat channel to signal that the game session is starting for the 
//		players in that StagingRoom.
//		<p>
//		After peerStartGame() is called, all of the other members in that 
//		StagingRoom will be notified by receiving a peerGameStartedCallback.
//		<p>
//		The value of the "message" argument is received in the 
//		peerGameStartedCallback, and can be used to pass information such 
//		as a special port or password required to connect to the game 
//		session.
//		<p>
//		Changes to individual player's ready flags are received in the 
//		peerReadyChangedCallback. An individual player's readiness state
//		can be queried with peerGetReady().
//		<p>
//		It is recommended that a StagingRoom game host not launch the game 
//		session with peerStartGame() until all clients show ready in 
//		peerAreAllReady(), but the Peer SDK does not require or enforce 
//		readiness. This function can be called by the StagingRoom host at 
//		any time, even if some or all players show not ready. 
//		<p>
// 		If you set the PEER_STOP_REPORTING bit flag as part of the 
//		reportingOptions argument, then Peer will stop reporting this
//		game host to the GameSpy Master Server and your game is then 
//		responsible for continuing to report the game host to the GameSpy 
//		Master Server, if desired, with a peerStartReporting() or
//		peerStartReportingWithSocket() call. 
//		<p>
// 		If, instead, you set the PEER_KEEP_REPORTING bit flag as part of 
//		the reportingOptions argument, then Peer will continue reporting 
//		this game host to the GameSpy Master Server and invoking the 
//		peerQR...() callbacks your game registered to provide these keys. 
//		<p>
//		Peer will normally not report the same information while playing
//		that it did while the StagingRoom was being advertised. 
//		<p>
// 		While playing, your game will be responsible for reporting 
//		the following host keys via callbacks: the gamemode (if not 
//		openplaying), the hostname, the numplayers, the maxplayers, and a 
//		password if any. 
//		<p>
//		However, if you set the PEER_REPORT_INFO bit flag as part of the 
//		reportingOptions argument, then Peer will automatically report 
//		these host keys as GAMEMODE_KEY (when not playing), HOSTNAME_KEY, 
//		NUMPLAYERS_KEY, MAXPLAYERS_KEY, and PASSWORD_KEY and 
//		peerQRServerKeyCallback will not be called for these key values.
//		<p>
//		Your game will also need to report the players and pings in the 
//		callbacks, unless you set the PEER_REPORT_PLAYERS bit flag as part
//		of reportingOptions argument. 
//		<p>
//		For more details, see the peerQR...Callback() functions.
//		<p>
//		Once a client joins a game session, we require the client halt 
//		further game updates by calling peerStopListingGames().
//		<p>
//      For Automatch, the host must call peerStartGame once in PEERReady status in order trigger 
//		the PEERComplete status for each player in the room.
// See Also
//		peerAreAllReady, peerGetReady, peerSetReady, peerReadyChangedCallback, 
//		peerSetTitle, peerStartListingGames, peerListingGamesCallback,
//		peerStopListingGames, peerJoinStagingRoom, 
//		peerQRServerKeyCallback, peerStartReporting, 
//		peerStartReportingWithSocket, peerGameStartedCallback
void peerStartGame
(
	PEER peer,					// The peer object.
	const gsi_char * message,	// A message to send to everyone.
	int reportingOptions		// Bitfield flags used to set reporting options.
);

///////////////////////////////////////////////////////////
// peerStartReporting
// Summary
//		Begin reporting the local client as a game host without 
//		creating a StagingRoom Chat channel.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns PEERTrue if reporting was successfully started or
//		PEERFalse otherwise.
// Remarks
//		The peerStartReporting function starts reporting the local client as 
//		a prospective game host without creating a StagingRoom Chat channel.
//		<p>
//		Call peerStopGame() when done to stop reporting.
// See Also
//		peerStartGame, peerStopGame, peerCreateStagingRoom,
//		peerCreateStagingRoomWithSocket, peerQRServerKeyCallback
PEERBool peerStartReporting
(
	PEER peer  // The peer object.
);

///////////////////////////////////////////////////////////
// peerStartReportingWithSocket
// Summary
//		Begin reporting the local client as a game host without 
//		creating a StagingRoom Chat channel but using the 
//		specified externally managed UDP socket.
// Parameters
//		peer	: [in] Initialized peer object.
//		socket	: [in] SOCKET to be used for reporting.
//		port	: [in] Local port that the socket is bound to.
// Returns
//		Returns PEERTrue if reporting was successfully started or
//		PEERFalse otherwise.
// Remarks
//		The peerStartReporting function starts reporting the local client as 
//		a prospective game host without creating a StagingRoom Chat channel
//		but using the specified externally managed UDP socket.
//		<p>
//		If peerStartReportingWithSocket is used, the socket provided must be 
//		a previously created UDP socket. It will be used for responding to 
//		query replies. Any Peer or QR2 queries the game reads from the socket 
//		must be handed to Peer using peerParseQuery(). Using a shared, 
//		externally managed socket is useful when running a game host behind a 
//		NAT proxy. For a full explanation of how this helps, see the "NAT and 
//		Firewall Support" Appendix in the "GameSpy Query and Reporting 2 SDK" 
//		documentation.
//		<p>
//		Call peerStopGame() when done to stop reporting.
//		<p>
//		A title must be set for this function to succeed, but Peer does not need 
//		to be connected. 
//		<p>
//		Also, this function cannot be called while already in a staging room. 
// See Also
//		peerStartGame, peerStopGame, peerCreateStagingRoom,
//		peerCreateStagingRoomWithSocket, peerQRServerKeyCallback
//		peerSetTitle
PEERBool peerStartReportingWithSocket
(
	PEER peer,				// The peer object.
	SOCKET socket,			// The socket to be used for reporting.
	unsigned short port		// The local port to which the socket is bound.
);

///////////////////////////////////////////////////////////
// peerStartPlaying
// Summary
//		Manually turn on the local player's "playing" flag in the 
//		event the local player joins a non-Peer game session.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerStartPlaying function manually turns on the local 
//		player's "playing" flag in the event the local player joins 
//		a non-Peer game session. This notifies other clients that 
//		the local client has joined a game session. 
//		<p>
//		Normally, the Peer SDK automatically handles this state 
//		change when the local client receives the 
//		peerGameStartedCallback after the host of a StagingRoom Chat 
//		channel calls peerStartGame().
// See Also
//		peerStartGame, peerGameStartedCallback, peerIsPlaying
void peerStartPlaying
(
	PEER peer  // The peer object.
);

///////////////////////////////////////////////////////////
// peerIsPlaying
// Summary
//		Returns PEERTrue if the local client's "playing" flag is turned on.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		This function returns PEERTrue if the local client's "playing" flag
//		is turned on or PEERFalse otherwise.
// Remarks
//		The peerIsPlaying functions checks if the local client's "playing" 
//		flag is turned on. The local client's "playing" flag is turned on
//		when the local client successfully launches a game with 
//		peerStartGame(), or when the local client is in a StagingRoom Chat
//		channel and receives the peerGameStartedCallback, or when the 
//		local client manually turns this flag on with peerStartPlaying().
//		<p>
//		This function can only be called if a game title is set and Peer 
//		is connected.
// See Also
//		peerStartGame, peerGameStartedCallback, peerStartPlaying,
//		peerSetTitle
PEERBool peerIsPlaying
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerStopGame
// Summary
//		Called by the game host when the game session has ended
//		to stop reporting.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerStopGame function is primarily used by a game host to stop 
//		reporting as a game host, but can also be used by an individual 
//		player to turn off their "playing" flag.
//		<p>
//		This function also does some Peer cleanup if the local client 
//		was the game host.
//		<p>
//		This should be called whenever returning from a game session.
// See Also
//		peerStartGame, peerGameStartedCallback, peerStartPlaying,
//		peerSetTitle
void peerStopGame
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerStateChanged
// Summary
//		When hosting, notify the GameSpy Master Server listing and prospective 
//		clients of a state or key change in this game host, such as when this
//		game host cannot accept any more players.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerStateChanged function should be called when you are hosting a 
//		game to force the GameSpy Master Server and prospective clients who 
//		may want to join your game to requery your game host keys.
//		<p>
//		This might happen when the game you are hosting changes mapname, 
//		level, or gametype, or when your game host has become full and 
//		cannot accept any new players.
//		<p>
//		You should only call peerStateChanged() when you are actively reporting 
//		as a game host, for example when you are hosting a StagingRoom after 
//		calling peerCreateStagingRoom() or peerCreateStagingRoomWithSocket() 
//		or if you are reporting a game without a StagingRoom after calling 
//		peerStartReporting() or peerStartReportingWithSocket().
//		<p>
//		A client that implements the Peer SDK and has your game host listed 
//		after having calling peerStartListingGames() will receive a 
//		peerListingGamesCallback with a PEER_UPDATE message to indicate the
//		peerStateChanged().
//		<p>
//		This routine should only be called when essential top-level data about
//		a game changes.
//		<p>
//		When the player count changes, peerStateChanged() is only called if 
//		the join ability of the games is affected (e.g. open -> full). Under
//		no circumstances should peerStateChanged() be called on a regular timer.
// See Also
//		peerCreateStagingRoom, peerCreateStagingRoomWithSocket, peerStartReporting,
//		peerStartReportingWithSocket, peerStartListingGames, 
//		peerListingGamesCallback, peerQRKeyListCallback
void peerStateChanged
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// Key Functions
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerSetGlobalKeys
// Summary
//		Set global keys on the local client player.
// Parameters
//		peer	: [in] Initialized peer object.
//		num		: [in] Number of keys in the keys array.
//		keys	: [in] Array of key names to set values for.
//		values	: [in] Array of corresponding key values to set.
// Remarks
//		The peerSetGlobalKeys function sets global keys on the local 
//		client player. At least one key must be set. 
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will do nothing if Peer is not connected.
// See Also
//		
void peerSetGlobalKeys
(
	PEER peer,					// The peer object.
	int num,					// The number of keys to set.
	const gsi_char ** keys,		// The key names to set.
	const gsi_char ** values	// The corresponding key values to set.
);

///////////////////////////////////////////////////////////
// peerGetPlayerGlobalKeys
// Summary
//		Query the Chat server for the global key values set on the
//		player specified by the nick argument.
// Parameters
//		peer		: [in] Initialized peer object.
//		nick		: [in] The chat nickname of the target player.
//		num			: [in] The number of keys in the array.
//		keys		: [in] Array of key names to request values for.
//		callback	: [in] Callback function call when the operation completes.
//		param		: [in] Pointer to user data. This is optional and will be 
//							passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not return 
//							until the operation has completed.
// Remarks
//		The peerGetPlayerGlobalKeys function queries the Chat server for the 
//		global key values set on the player specified by the nick argument.
//		<p>
//		You will receive these key values in the peerGetGlobalKeysCallback
//		function that you specify in the callback argument if the request
//		is successful.
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
// See Also
//		peerGetGlobalKeysCallback, peerSetGlobalKeys, peerGetRoomGlobalKeys
//		peerSetRoomKeys, 
void peerGetPlayerGlobalKeys
(
	PEER peer,							// The peer object.
	const gsi_char * nick,				// The player to get the keys for.
	int num,							// The number of keys.
	const gsi_char ** keys,				// The keys to get.
	peerGetGlobalKeysCallback callback,	// Called with the keys.
	void * param,						// Passed to callback.
	PEERBool blocking					// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerGetRoomGlobalKeys
// Summary
//		Query the Chat server for the global key values set on every 
//		player in the Chat channel room of the type specified in the
//		roomType argument that the local client is also a member of.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		num			: [in] Number of keys in the keys array argument.
//		keys		: [in] Array of key names to retrieve values for.
//		callback	: [in] Callback function to be called when the 
//							operation completes.
//		param		: [in] Pointer to user data.  This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue this function will not 
//							return until the operation has completed.
// Remarks
//		The peerGetRoomGlobalKeys function queries the Chat server for the 
//		global key values set on every player currently in one of the 
//		local client's three possible Chat channel rooms (TitleRoom, 
//		GroupRoom, or StagingRoom). The local client must be a member 
//		of a Chat channel room of the specified type. You specify which 
//		of the local client's three possible Chat channel rooms (TitleRoom, 
//		GroupRoom, or StagingRoom) to query player global keys in with
//		the roomType argument value. A player can only be in one TitleRoom, 
//		one GroupRoom, and one StagingRoom at a time, so the roomType 
//		argument is sufficient to select which Chat channel room to query.
//		<p>
//		The specified peerGetPlayerGlobalKeysCallback will be called once 
//		for each player in that Chat channel room to return the results of 
//		the query, then once more with "nick" set to NULL. 
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function does nothing if no game title is set or if Peer is 
//		not connected.
// See Also
//		peerGetPlayerGlobalKeysCallback, peerGetPlayerGlobalKeys, 
//		peerSetTitle
void peerGetRoomGlobalKeys
(
	PEER peer,							// The peer object.
	RoomType roomType,					// The room to get the keys in.
	int num,							// The number of keys.
	const gsi_char ** keys,				// The keys to get.
	peerGetGlobalKeysCallback callback, // Called with the keys.
	void * param,						// Passed to callback.
	PEERBool blocking					// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerSetRoomKeys
// Summary
//		Set room keys for the player specified by the nick argument or 
//		set room keys on the room itself for the Chat channel room of 
//		the type specified in the roomType argument that the local 
//		client is the host or operator of.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] Chat nickname of the target player or NULL or 
//							"" for the room.
//		num			: [in] Number of keys to set.
//		keys		: [in] Array of key names to set values for.
//		values		: [in] Array of key values to set, corresponding to the 
//							key names in the previous argument.
// Remarks
//		If the nick argument is NULL or "", the peerSetRoomKeys function 
//		sets the room keys for the Chat server for one of the local 
//		client's three possible Chat channel rooms (TitleRoom, GroupRoom, 
//		or StagingRoom). The local client must be the operator or host of 
//		a Chat channel room of the specified type. You specify which of 
//		the local client's three possible Chat channel rooms (TitleRoom, 
//		GroupRoom, or StagingRoom) to set the room keys for with the 
//		roomType argument value. A player can only be in one TitleRoom, 
//		one GroupRoom, and one StagingRoom at a time, so the roomType 
//		argument is sufficient to select which Chat channel room.
//		<p>
//		Otherwise, if the nick argument is not NULL or "" and the nick 
//		also matches the Chat nickname of a player in a Chat channel room
//		of the type specified by the roomType argument in which the local
//		client is a host or an operator, then the specified keys will be 
//		set on the player specified by the nick argument.
//		<p>
//		If you want a key to be broadcast immediately to all members of the 
//		current room you must prefix that key name with "b_".
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will fail if no game title is set or if Peer is not 
//		connected.
// See Also
//		peerGlobalKeyChangedCallback, peerRoomKeyChangedCallback, 
//		peerGetRoomKeys, peerGetRoomGlobalKeys, peerGetPlayerGlobalKeys
//		peerSetTitle
void peerSetRoomKeys
(
	PEER peer,					// The peer object.
	RoomType roomType,			// The room to set the keys in.
	const gsi_char * nick,		// The player to set the keys on (NULL 
								// or "" for the room).
	int num,					// The number of keys.
	const gsi_char ** keys,		// The keys names to set.
	const gsi_char ** values	// The corresponding key values to set.
);

///////////////////////////////////////////////////////////
// peerGetRoomKeys
// Summary
//		Queries the Chat server for key values for the specified player, 
//		or for all players in a Chat channel room, or for key values on 
//		the room itself when the local client is a member of that room 
//		type as specified by the roomType argument.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick		: [in] Nickname of the player to retrieve values on. 
//							"*" to retrieve values for all players. 
//							"" or NULL to retrieve values on the room 
//							itself.
//		num			: [in] Number of valid key names in the keys array 
//							argument.
//		keys		: [in] Array of key names to retrieve values for.
//		callback	: [in] Callback function to call when the operation 
//							completes.
//		param		: [in] Pointer to user data. This is optional and will 
//							be passed unmodified to the callback function.
//		blocking	: [in] When set to PEERTrue, this function will not 
//							return until the operation has completed.
// Remarks
//		If the nick argument is NULL or "", the peerSetRoomKeys function 
//		queries the Chat server for room keys for one of the local 
//		client's three possible Chat channel rooms (TitleRoom, GroupRoom, 
//		or StagingRoom). The local client must be a member of a Chat 
//		channel room of the specified type. You specify which of the 
//		local client's three possible Chat channel rooms (TitleRoom, 
//		GroupRoom, or StagingRoom) to get room keys for with the roomType
//		argument value. A player can only be in one TitleRoom, one 
//		GroupRoom, and one StagingRoom at a time, so the roomType 
//		argument is sufficient to select which Chat channel room.
//		<p>
//		Otherwise, if the nick argument specifies the Chat nickname of a 
//		player in a Chat channel room of the type specified by the 
//		roomType argument that the local client also a member of, then 
//		the specified player keys will be queried for the player 
//		with that nick.
//		<p>
//		Otherwise, if the nick argument is "*" and the local client is
//		a member in a Chat channel room of the type specified by the 
//		roomType argument, then the specified player keys will be queried 
//		for every player in that Chat channel room.
//		<p>
//		In usage 1 or 2, the peerGetRoomKeysCallback will be called once
//		to report the results of the queried room or player keys.
//		<p>
//		In the third use case, the peerGetRoomKeysCallback will be called 
//		once for each player in that Chat channel room to report the 
//		results of the queried player keys, then one more time with nick 
//		set to NULL.
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will function fail if no game title is set or if 
//		Peer is not connected.
//		<p>
//		This function should not be called excessively.  The game should
//		maintain a cache of the current key values.
//		<p>
//		When entering a group room(lobby), peerGetRoomKeys() needs to only
//		be called once (with the nickname "*"), instead of once per player.
// See Also
//		peerGetRoomKeysCallback, peerGetRoomGlobalKeys, 
//		peerSetRoomWatchKeys, peerGetRoomWatchKey, peerSetTitle
void peerGetRoomKeys
(
	PEER peer,							// The peer object.
	RoomType roomType,					// The room to get the keys in.
	const gsi_char * nick,				// The player to get the keys for.
	int num,							// The number of keys.
	const gsi_char ** keys,				// The keys to get.
	peerGetRoomKeysCallback callback,	// Called with the keys.
	void * param,						// Passed to callback.
	PEERBool blocking					// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerSetGlobalWatchKeys
// Summary
//		(Deprecated function - use Room Keys/Global Keys instead) Add to or replace the list of global player keys to query and 
//		cache whenever entering a room of the type specified by the 
//		roomType argument.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		num			: [in] Number of keys in the array.
//		keys		: [in] Array of key names to query when entering.
//		addKeys		: [in] When set to PEERTrue this keys will be added 
//							to the existing watch key list.
// Remarks
//		The peerSetGlobalWatchKeys function adds to or replaces the 
//		list of global player keys to query and cache whenever entering 
//		a room of the type specified by the roomType argument.
//		<p>
//		If the addKeys argument is set to PEERTrue, the values in the 
//		keys array argument will be added to the current list of player
//		key names to always query and cache when entering the specified 
//		roomType.
//		<p>
//		If the addKeys argument is set to PEERFalse, the values in the 
//		keys array argument will replace the existing list of player 
//		key names to always query and cache when entering the specified 
//		roomType.
//		<p>
//		To get a player key value queried and cached in this way, call 
//		the peerGetGlobalWatchKey() function.
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will fail if no game title is set or if Peer is not 
//		connected.
// See Also
//		peerGetGlobalWatchKey, peerGetPlayerGlobalKeys, peerSetTitle
void peerSetGlobalWatchKeys
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room to set the watch keys for.
	int num,				// The number of keys.
	const gsi_char ** keys, // The key names to watch for.
	PEERBool addKeys		// If PEERTrue, add these keys to the existing 
							//	global watch keys for this room.
);

///////////////////////////////////////////////////////////
// peerGetGlobalWatchKey
// Summary
//		(Deprecated function - use Room Keys/Global Keys instead) Returns the cached value of the watched player key specified
//		by the nick and key arguments.
// Parameters
//		peer	: [in] Initialized peer object.
//		nick	: [in] The Chat nickname of the player.
//		key		: [in] The name of the key.
// Returns
//		The watch key's value, or NULL if the watch key is unknown. 
//		The empty string "" is a legal key value.
// Remarks
//		The peerGetGlobalWatchKey function may be used to get the cached
//		value of an automatically queried global player key. If the key 
//		value is not known or has not been received this function will 
//		return NULL. Note that an empty string "" is valid key value. If 
//		the key is just empty (or was never set), an empty string will be 
//		returned.
//		<p>
//		The key being requested must have previously been set as a global 
//		watch key with peerSetGlobalWatchKey() for a room that the player
//		specified by nick and the local client have in common. 
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will fail if no game title is set.
// See Also
//		peerSetGlobalWatchKeys, peerGetPlayerGlobalKeys, peerSetTitle
const gsi_char * peerGetGlobalWatchKey
(
	PEER peer,				// The peer object.
	const gsi_char * nick,  // The player to get the key for.
	const gsi_char * key	// The key to get.
 );

///////////////////////////////////////////////////////////
// peerSetRoomWatchKeys
// Summary
//		(Deprecated function - use Room Keys/Global Keys instead) Add to or replace the list of room keys to query and cache
//		whenever entering a room of the type specified by the roomType 
//		argument.
// Parameters
//		peer		: [in] Initialized peer object.
//		roomType	: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		num			: [in] Number of keys in the array.
//		keys		: [in] Array of key names to query when entering.
//		addKeys		: [in] When set to PEERTrue this keys will be added 
//							to the existing watch key list.
// Remarks
//		The peerSetRoomWatchKeys function adds to or replaces the 
//		list of global room keys to query and cache whenever entering 
//		a room of the type specified by the roomType argument.
//		<p>
//		If the addKeys argument is set to PEERTrue, the values in the 
//		keys array argument will be added to the current list of room
//		key names to always query and cache when entering the specified 
//		roomType.
//		<p>
//		If the addKeys argument is set to PEERFalse, the values in the 
//		keys array argument will replace the existing list of room key 
//		names to always query and cache when entering the specified 
//		roomType.
//		<p>
//		To get a room key value queried and cached in this way, call 
//		the peerGetRoomWatchKey() function.
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will fail if no game title is set or if Peer is not 
//		connected.
// See Also
//		peerGetRoomWatchKey, peerGetRoomKeys, peerSetTitle
void peerSetRoomWatchKeys
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room to set the watch keys for.
	int num,				// The number of keys.
	const gsi_char ** keys, // The keys to watch for.
	PEERBool addKeys		// If PEERTrue, add these keys to the existing 
							// room watch keys for this room.
);

///////////////////////////////////////////////////////////
// peerGetRoomWatchKey
// Summary
//		(Deprecated function - use Room Keys/Global Keys instead) Returns the cached value of the watched room key specified
//		by the roomType, nick, and key arguments.
// Parameters
//		peer	: [in] Initialized peer object.
//		roomType: [in] One of TitleRoom, GroupRoom or StagingRoom.
//		nick	: [in] The Chat nickname of the player.
//		key		: [in] The name of the key.
// Returns
//		The watch key's value, or NULL if the watch key is unknown. 
//		The empty string "" is a legal key value.
// Remarks
//		The peerGetRoomWatchKey function may be used to get the cached
//		value of an automatically queried room key. If the key value 
//		is not known or has not been received this function will return 
//		NULL. Note that an empty string "" is valid key value. If the 
//		key is just empty (or was never set), an empty string will be 
//		returned.
//		<p>
//		The key being requested must have previously been set as a room 
//		watch key with peerSetGlobalWatchKey() for the specified 
//		roomType.
//		<p>
//		Non-ASCII Unicode characters in key names or key values will be 
//		changed to underscores.
//		<p>
//		This function will fail if no game title is set.
// See Also
//		peerSetRoomWatchKeys, peerGetRoomKeys, peerSetTitle
////////////////////////////////////////////////////////////
const gsi_char * peerGetRoomWatchKey
(
	PEER peer,				// The peer object.
	RoomType roomType,		// The type of room to get the key from.
	const gsi_char * nick,  // The player to get the key for.
	const gsi_char * key	// The name of the key to get.
);

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// AutoMatch Functions
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// peerStartAutoMatch
// Summary
//		Start an AutoMatch attempt.
// Parameters
//		peer			: [in] Initialized peer object.
//		maxPlayers		: [in] Total number of players to match. (Includes 
//								the local player.)
//		filter			: [in] Hard filter for returned server list.
//		statusCallback	: [in] Callback function to be called when the status 
//								changes.
//		rateCallback	: [in] Callback function to be called when a potential 
//								match should be rated.
//		param			: [in] Pointer to user data. This is optional and will 
//								be passed unmodified to the callback function.
//		blocking		: [in] When set to PEERTrue this function will not 
//								return until the operation has completed.
// Remarks
//		The peerStartAutoMatch function begins the AutoMatch process.
//		<p>
//		The function takes a maxPlayers argument, which specifies the maximum 
//		number of people that should be in the final match. For example, if the 
//		local player wants to play a 3v3 match, maxPlayers should be 6. 
//		<p>
//		The filter is a SQL-style filter, just like the filter used in 
//		peerStartListingGames(). It is used to rule out matches that are not 
//		suitable. Potential matches that do not pass the filter will not be 
//		passed to the rating callback. 
//		<p>
//		The statusCallback is called whenever the status of the match changes, 
//		until either peerStopAutoMatch() is called, or the statusCallback is 
//		called with a status of PEERFailed or PEERComplete. Note: once in PEERReady state
//      the host must call peerStartGame, which will trigger the PEERComplete status for each player.
//		<p>
//		The rateCallback is used to rate possible matches.
//		<p>
//		The filter contains any hard criteria.  This is used to narrow down the 
//		list of possible matches to those the user might consider.
//		<p>
//		To use Nat Negotiation with Automatch, make sure to wait until PEERComplete status
//      (after the host has called PeerStartGame) then use peerUTMPlayer to share your NN 
//      cookie with the host. *Do not* use peerSendNatNegotiateCookie when automatching.
//      If you are the host, receive the client cookies via the peerPlayerUTMCallback then 
//      coordinate Nat Negotiation with each client accordingly.
// See Also
//		peerStartAutoMatchWithSocket, peerStopAutoMatch, 
//		peerAutoMatchStatusCallback, peerAutoMatchRateCallback, 
//		peerStartListingGames, peerIsAutoMatching, peerGetAutoMatchStatus
void peerStartAutoMatch
(
	PEER peer,									// The peer object.
	int maxPlayers,								// The total number of players to 
												// match (including the local player).
	const char * filter,					// Hard criteria - filters out servers.
	peerAutoMatchStatusCallback statusCallback,	// Called as the attempt status changes.
	peerAutoMatchRateCallback rateCallback,		// Called to rate potential matches.
	void * param,								// User-data.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerStartAutoMatchWithSocket
// Summary
//		Start an AutoMatch attempt using an externally managed socket.
// Parameters
//		peer			: [in] Initialized peer object.
//		maxPlayers		: [in] Total number of players to match. (Includes 
//								the local player.)
//		filter			: [in] Hard filter for returned server list.
//		socket			: [in] Socket to be used for reporting.
//		port			: [in] Local port to which the socket is bound.
//		statusCallback	: [in] Callback function to be called when the status 
//								changes.
//		rateCallback	: [in] Callback function to be called when a potential 
//								match should be rated.
//		param			: [in] Pointer to user data. This is optional and will 
//								be passed unmodified to the callback function.
//		blocking		: [in] When set to PEERTrue this function will not 
//								return until the operation has completed.
// Remarks
//		The peerStartAutoMatch function begins the AutoMatch process using the 
//		specified externally managed socket. The function takes a maxPlayers 
//		argument, which specifies the maximum number of people that should be 
//		in the final match. For example, if the local player wants to play a 
//		3v3 match, maxPlayers should be 6.
//		<p>
//		The filter is a SQL-style filter, just like the filter used in 
//		peerStartListingGames(). It is used to rule out matches that are not 
//		suitable. Potential matches that do not pass the filter will not be 
//		passed to the rating callback. 
//		<p>
//		The statusCallback is called whenever the status of the match changes, 
//		until either peerStopAutoMatch() is called, or the statusCallback is 
//		called with a status of PEERFailed or PEERComplete. Note: once in PEERReady state
//      the host must call peerStartGame, which will trigger the PEERComplete status for each player. 
//		<p>
//		The rateCallback is used to rate possible matches.
//		<p>
//		The filter contains any hard criteria.  This is used to narrow down the 
//		list of possible matches to those the user might consider.
//		<p>
//		This function allows the game to share a socket with the Peer SDK, 
//		which can make hosting games behind a NAT proxy more feasible.
//		<p>
//		To use Nat Negotiation with Automatch, make sure to wait until PEERComplete status
//      (after the host has called PeerStartGame) then use peerUTMPlayer to share your NN 
//      cookie with the host. *Do not* use peerSendNatNegotiateCookie when automatching.
//      If you are the host, receive the client cookies via the peerPlayerUTMCallback then 
//      coordinate Nat Negotiation with each client accordingly.
// See Also
//		peerStartAutoMatch, peerStopAutoMatch, 
//		peerAutoMatchStatusCallback, peerAutoMatchRateCallback, 
//		peerStartListingGames, peerIsAutoMatching, peerGetAutoMatchStatus
void peerStartAutoMatchWithSocket
(
	PEER peer,									// The peer object.
	int maxPlayers,								// The total number of players to 
												// match (including the local player).
	const char * filter,					// Hard criteria - filters out servers.
	SOCKET socket,								// The socket to be used for reporting.
	unsigned short port,						// The local port to which the socket is bound.
	peerAutoMatchStatusCallback statusCallback,	// Called as the attempt status changes.
	peerAutoMatchRateCallback rateCallback,		// Called to rate potential matches.
	void * param,								// User-data.
	PEERBool blocking							// If PEERTrue, don't return until finished.
);

///////////////////////////////////////////////////////////
// peerStopAutoMatch
// Summary
//		Stop the current AutoMatch attempt in progress.
// Parameters
//		peer	: [in] Initialized peer object.
// Remarks
//		The peerStopAutoMatch function may be used to cancel the 
//		AutoMatch attempt currently in progress. This is generally 
//		used so that a user may cancel the AutoMatch process if a 
//		suitable match has not been found quickly enough, which can
//		result from the user specifying very narrow search criteria.
// See Also
//		peerStartAutoMatch, peerStartAutoMatchWithSocket
//		peerIsAutoMatching, peerGetAutoMatchStatus
void peerStopAutoMatch
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerIsAutoMatching
// Summary
//		Returns PEERTrue if an AutoMatch attempt is in progress.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		Returns PEERTrue if an AutoMatch attempt is in progress or 
//		PEERFalse otherwise.
// Remarks
//		The peerIsAutoMatching function is used to determine if an 
//		AutoMatch attempt is in progress. It returns PEERTrue if an 
//		AutoMatch attempt is in progress or PEERFalse if otherwise.
// See Also
//		peerStartAutoMatch, peerStartAutoMatchWithSocket,
//		peerStopAutoMatch, peerGetAutoMatchStatus
PEERBool peerIsAutoMatching
(
	PEER peer		// The peer object.
);

///////////////////////////////////////////////////////////
// peerGetAutoMatchStatus
// Summary
//		Used when AutoMatching to retrieve the current status.
// Parameters
//		peer	: [in] Initialized peer object.
// Returns
//		The current AutoMatch status. See remarks.
// Remarks
//		The peerGetAutoMatchStatus function returns the current AutoMatch 
//		status. The return value is one of the PEERAutoMatchStatus enumerated 
//		type values.
// See Also
//		peerStartAutoMatch, peerStartAutoMatchWithSocket,
//		peerStopAutoMatch, peerIsAutoMatching
PEERAutoMatchStatus peerGetAutoMatchStatus
(
	PEER peer		// The peer object.
);


#ifdef __cplusplus
}
#endif

#endif
