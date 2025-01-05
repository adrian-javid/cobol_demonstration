"use strict";

var Module = {
	print: (function() {
		return (...args) => {
			const text = args.join(' ');
			console.log(text);
		};
	})(),
	printErr: function(text) {
		if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
		console.error(text);
	},
	canvas: (() => {
		const canvas = document.getElementById('canvas');
		canvas.addEventListener("webglcontextlost", (event) => {
				event.preventDefault();
				alert("The Web Graphics Library context has been lost. The page will need to reload to restore functionality.");
				window.location.reload();
			},
			false
		);
		return canvas;
	})(),
};

function userAgentIsGenuineGecko() {
	const userAgent = window.navigator.userAgent;
	return userAgent.includes("Gecko") && !(userAgent.includes("like Gecko"));
}

function userAgentIsHandheld() {
	const userAgent = window.navigator.userAgent;
	return /Mobile|Tablet|Android|iPhone|iPad|iPod|Windows Phone|BlackBerry|BB10|Kindle|Silk/i.test(userAgent);
}

function fallbackForGecko() {
	// unimplemented
}

function onApplicationInitialized() {
	if (userAgentIsGenuineGecko() && !(userAgentIsHandheld())) try {
		fallbackForGecko();
	} catch(error) {
		console.error("Failed to use fallback for Gecko. ", error);
	}

	const loadingMessage = document.getElementById("loading_message");

	// Hide the loading message.
	if (loadingMessage) loadingMessage.style.display = "none";
}
