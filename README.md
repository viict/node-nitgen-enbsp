# NITGEN ENBSP Module for Nodejs

Digital fingerprint reader module for NodeJS using a Nitgen device.

Currently tested on Windows with NodeJS 16 (x86), Python 3.10 (x86) and VS 2019 (x64/x86)

Based on the awesome work from [Paulo Roberto](https://github.com/xxpauloxx/nodejs-nbiobsp)

## Usage: as dependency

```bash
yarn add @viict/node-nitgen-enbsp

```
#### Simple code

```javascript
const enbsp = require('@viict/node-nitgen-enbsp');
const init = enbsp.init();

let fir1, fir2;
if(init === enbsp.NBioAPIERROR_NONE){
	console.log("Insert the first fingerprint: ");
	fir1 = enbsp.capture(4000);

	console.log("Insert the second fingerprint: ");
	fir2 = enbsp.capture(4000);
}

console.log(`Match: `, enbsp.match(fir1, fir2) === true);
enbsp.close()
```

## Usage: cloning repo

```bash
git clone https://github.com/viict/node-nitgen-enbsp.git
cd node-nitgen-enbsp
yarn
yarn test
```

#### Simple code

```javascript
const enbsp = require('./index.js');
const init = enbsp.init();

let fir1, fir2;
if(init == enbsp.NBioAPIERROR_NONE){
	console.log("Insert the first fingerprint: ");
	fir1 = enbsp.capture(4000);

	console.log("Insert the second fingerprint: ");
	fir2 = enbsp.capture(4000);
}

console.log(`Match: `, enbsp.match(fir1, fir2) === true);
enbsp.close()
```

> Check the test.js for more examples

#### Any problem? Open a new issue
