///////////////////////////////////////////////////////////////////////////////
// File:	gcdkeys.h
// SDK:		GameSpy CD Key SDK
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#ifndef _GOACDKEYS_H_
#define _GOACDKEYS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GUSE_ASSERTS

/*****
QR2CDKEY_INTEGRATION: This define controls where the functions needed to integrate
the networking of the Query & Reporting 2 SDK and CDKey SDKs are available.
If you intend to use the integration option for these SDKs, you must uncomment the 
define below, or provide it as an option to your compiler.
*******/
//#define QR2CDKEY_INTEGRATION

//////////////////////////////////////////////////////////////
// AuthCallBackFn
// Summary
//		Called when the user's CD key is either authorized or rejected.
// Parameters
//		gameid			: [in] The game ID for which authentication is requested.
//		localid			: [in] The id that was passed into gcd_authenticate_user.
//		authenticated	: [in] Indicates whether the user was authenticated: 1 if 
//								authenticated; 0 if not.
//		errmsg			: [in] Error message if user was not authenticated.
//		instance		: [in] The same instance as was passed into the gcd_authenticate_user.
// Remarks
//		This function will be called within two seconds of gcd_authenticate_user, 
//		even if the validation server hasn't responded yet.
//		<p>
//		If the authentication failed, one of the following errmsg strings will
//		be received:
//		<p>
//		<emit \<dl\>>
//			<emit \<dt\>>
//		"Bad Response"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key was incorrect. Check that the CD Key was correctly typed 
//			or passed to the compute response function. Make sure the 
//			gcd_authenticate_user is passed the correct values.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Invalid CD Key"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key is not registered for this game on the GameSpy backend.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Invalid authentication"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			Either the CD Key was bad or the response and challenge were bad. 
//			Make sure the gcd_authenticate_user is passed the correct values.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Your CD Key is disabled. Contact customer service."
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The specific, provided CD key value has been turned off.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"CD Key in use"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The CD Key provided was in use by another player.
//				<emit \</dd\>>
//			<emit \<dt\>>
//		"Validation Timeout"
//			<emit \</dt\>>
//				<emit \<dd\>>
//			The host was not able to reach the CD key validation server. The 
//			SDK intentionally authenticates the user in this case, since it
//			would not be desirable to reject players without network connectivity.
//				<emit \</dd\>>
//		<emit \</dl\>>
// <p>
// See Also
//		gcd_authenticate_user
typedef void (*AuthCallBackFn)(
	int gameid, 
	int localid, 
	int authenticated, 
	char *errmsg, 
void *instance
);

//////////////////////////////////////////////////////////////
// RefreshAuthCallBackFn
// Summary
//		Used to reauthenticate a client for the purpose of proving a client is still online.
// Parameters
//		gameid		: [in] the game ID used to initialize the SDK with
//		localid		: [in] the index of the player
//		hint		: [in] a session id for a client used for reauthentication - 
//							this is the skey passed into gcd_process_reauth
//		challenge	: [in] a challenge string used for reauthentication
//		instance	: [in] user data passed in gcd_authenticate_user
// Remarks
//		The reauthentication callback will be called any time the validation server
//		wishes to determine if a client is still online.
//		When called, the client index, challenge, and session key will be available.
//		These values must be used to reauthenticate the user.
//		Remember that this process is similar to the primary authentication process, where the only difference is 
//		that the validation server provides the challenge and session key (note: the "hint" parameter 
//		in this callback is the session key that should be passed as the "skey" value into gcd_process_reauth).<p>
//		If the user was not authenticated, the errmsg parameter contains a descriptive string of the reason 
//		(either CD Key not valid, or CD Key in use).
typedef void (*RefreshAuthCallBackFn)(
	int gameid, 
	int localid, 
	int hint, 
	char *challenge, 
	void *instance
);

/* The hostname of the validation server.
If the app resolves the hostname, an
IP can be stored here before calling
gcd_init */
extern char gcd_hostname[64];

//////////////////////////////////////////////////////////////
// gcd_init
// Summary
//		Initializes the Server API and creates the sockets and structures.
// Parameters
//		gameid		: [in] The game ID issued for your game.
// Returns
//      Returns 0 if successful; non-zero if error. 
// Remarks
//      Should only be called once (unless gcd_shutdown has been called).
int gcd_init(int gameid);


#ifdef QR2CDKEY_INTEGRATION

#include "../qr2/qr2.h"

//////////////////////////////////////////////////////////////
// gcd_init_qr2
// Summary
//		Initializes the Server API and integrates the networking of the CDKey SDK with the Query & Reporting 2 SDK.
// Parameters
//		qrec		: [in] The intialized QR2 SDK object.
//		gameid		: [in] The game ID issued for your game.
// Returns
//      Returns 0 if successful; non-zero if error. 
// Remarks
//      You must initialize the Query & Reporting 2 SDK with qr2_init or qr2_init_socket prior to calling this. 
//      If you are using multiple instances of the QR2 SDK, you can pass the specific instance information in 
//      via the "qrec" argument. Otherwise you can simply pass in NULL.<p>
//		Make sure to use this function instead of the deprecated gcd_init() function.  This is mandatory to pass
//		source review certification.
int gcd_init_qr2(qr2_t qrec, int gameid);

#endif

//////////////////////////////////////////////////////////////
// gcd_shutdown
// Summary
//		Release the socket and send disconnect messages to the validation server for any clients still on the server.
void gcd_shutdown(void);

//////////////////////////////////////////////////////////////
// gcd_authenticate_user
// Summary
//		Creates a new client and sends a request for authorization to the validation server.
// Parameters
//		gameid		: [in] The game ID issued for your game.
//		localid		: [in] A unique int used to identify each client on the server. 
//							No two clients should have the same localid.
//		userip		: [in] The client's IP address, preferably in network byte order.
//		challenge	: [in] The challenge string that was sent to the client. Should be no more than 32 characters.
//		response	: [in] The response that the client received.
//		authfn		: [in] A callback that is called when the user is either authorized or rejected.
//		refreshfn	: [in] A callback called when the server needs to re-authorize a client on the local host
//		instance	: [in] Optional free-format user data for use by the callback.
// Remarks
//		If host self-authorization is being used then the recommended way of implementing host authentication is
//		through qr2_register_publicaddress_callback.  This is recommend due to an issue with port forwarding on 
//		the hosts end that ends up blocking communication from CD Key backend.
// See Also
//		qr2_register_publicaddress_callback
void gcd_authenticate_user(
int gameid, 
int localid,
unsigned int userip, 
const char *challenge, 
const char *response,
AuthCallBackFn authfn, 
RefreshAuthCallBackFn refreshfn, 
void *instance
);

//////////////////////////////////////////////////////////////
// gcd_process_reauth
// Summary
//		Used to respond to a reauthentication request made by the validation server proving the client is still on.
// Parameters
//		gameid		: [in] The game ID used to initialize the SDK with
//		localid		: [in] An index of the client
//		skey		: [in] The client's session key that came from the validation server
//		response	: [in] The client's response to the challenge
// Remarks
//		When the Reauthentication callback (passed to gcd_ authenticate user) is called, 
//		the host/server must send the required information to verify that the client is still online, 
//		using the CD Key being checked.
//		This should be called after the client has computed a response to the challenge coming from the callback.<p>
void gcd_process_reauth(int gameid, int localid, int hint, const char *response);


//////////////////////////////////////////////////////////////
// gcd_disconnect_user
// Summary
//		Notify the validation server that a user has disconnected.
// Parameters
//		gameid	: [in] The game ID issued for your game.
//		localid	: [in] The unique int used to identify the user.
void gcd_disconnect_user(int gameid, int localid);


//////////////////////////////////////////////////////////////
// gcd_disconnect_all
// Summary
//		Calls gcd_disconnect_user for each user still online.
// Parameters
//		gameid	: [in] The game ID issued for your game.
void gcd_disconnect_all(int gameid);

//////////////////////////////////////////////////////////////
// gcd_think
// Summary
//		Processes any pending data from the validation server and calls the callback to indicate 
//		whether a client was authorized or not.
// Remarks
//		This function should be called at least once every 10-100ms and is guaranteed not 
//		to block (although it may make a callback if an authorization response has come in). 
//		If your game uses the Query and Reporting SDK, you can place this call in the same area as 
//		the call to qr_process_queries.<p>
void gcd_think(void);

//////////////////////////////////////////////////////////////
// gcd_getkeyhash
// Summary
//		Returns the key hash for the given user.
// Parameters
//		gameid	: [in] The game ID issued for your game.
//		localid	: [in] The unique int used to identify the user.
// Returns
//		Returns the key hash string, or an empty string if that user is not connected.
// Remarks
//		The hash returned will always be the same for a given user. 
//		This makes it useful for banning or tracking of users (used with the Tracking/Stats SDK).
//		Returns an empty string if that user isn't connected.<p>
char *gcd_getkeyhash(int gameid, int localid);

#ifdef __cplusplus
}
#endif

#endif
