console.log(" ");

const nitgenBSP = require('./index.js');
console.warn(nitgenBSP);
const init = nitgenBSP.init();
// nitgenBSP.setSkin("NBSP2Jpn.dll"); // Japanese
// nitgenBSP.setSkin("NBSP2Kor.dll"); // Korean
// nitgenBSP.setSkin("NBSP2Por.dll"); // Portuguese

let fir1, fir2;
console.warn(`init: ${init}`);
if(init === nitgenBSP.NBioAPIERROR_NONE){
	console.log("Enroll the fingerprints you may want: ");
	fir1 = nitgenBSP.capture(4000, nitgenBSP.NBioAPI_FIR_PURPOSE_ENROLL);
	if (typeof fir1 === 'number') {
		switch(fir1) {
			case nitgenBSP.NBioAPIERROR_USER_CANCEL:
				console.warn('User canceled');
				process.exit(fir1);
			case nitgenBSP.NBioAPIERROR_CAPTURE_TIMEOUT:
				console.warn('Capture timeout');
				process.exit(fir1);
		}
	}
	console.log('First fingerprint:', fir1);

	console.log("Capture a new fingerprint to verify against enrolled: ");
	fir2 = nitgenBSP.capture(4000, nitgenBSP.NBioAPI_FIR_PURPOSE_VERIFY);
	if (typeof fir2 === 'number') {
		switch(fir2) {
			case nitgenBSP.NBioAPIERROR_USER_CANCEL:
				console.warn('User canceled');
				process.exit(fir2);
			case nitgenBSP.NBioAPIERROR_CAPTURE_TIMEOUT:
				console.warn('Capture timeout');
				process.exit(fir2);
		}
	}
	console.log('Second fingerprint:', fir2);

	const match = nitgenBSP.match(fir1, fir2);
	switch(match) {
		case nitgenBSP.NBioAPIERROR_NONE:
			console.log('Captured fingerprint compared to enrolled ones: NOT MATCHED', match);
		break;
		case nitgenBSP.NBioAPIERROR_DATA_PROCESS_FAIL:
		case nitgenBSP.NBioAPIERROR_MUST_BE_PROCESSED_DATA:
		case nitgenBSP.NBioAPIERROR_UNKNOWN_INPUTFORMAT:
		case nitgenBSP.NBioAPIERROR_UNKNOWN_FORMAT:
			console.log('Captured fingerprint compared to enrolled ones: INVALID DATA', match);
		break;
		case nitgenBSP.NBioAPIERROR_INTERNAL_CHECKSUM_FAIL:
			console.log('Captured fingerprint compared to enrolled ones: DATA LOOKS FORGED', match);
		break;
		case true:
			console.log('Captured fingerprint compared to enrolled ones: MATCHED', match);
	}

	console.log('Close device: ', nitgenBSP.close());
} else {
	console.log("Device disconnected or had error during initialization.");
}
