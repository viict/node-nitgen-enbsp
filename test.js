console.log(" ");

const nitgenBSP = require('./index.js');
const init = nitgenBSP.init();
// nitgenBSP.setSkin("NBSP2Jpn.dll"); // Japanese
// nitgenBSP.setSkin("NBSP2Kor.dll"); // Korean
// nitgenBSP.setSkin("NBSP2Por.dll"); // Portuguese

if(init == true){
	console.log("Enroll the fingerprints you may want: ");
	const fir1 = nitgenBSP.capture(4000, nitgenBSP.NBioAPI_FIR_PURPOSE_ENROLL);
	console.log(fir1);

	console.log("Capture a new fingerprint to verify against enrolled: ");
	const fir2 = nitgenBSP.capture(4000, nitgenBSP.NBioAPI_FIR_PURPOSE_VERIFY);
	console.log(fir2);
}

console.log('Captured fingerprint compared to enrolled ones: ', nitgenBSP.match(fir1, fir2));
nitgenBSP.close()

