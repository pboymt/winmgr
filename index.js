const join = require('path').join;

const winmgr = require(join(__dirname, 'build/Release/winmgr.node'));

const {
    getWindowInfoList,
    focusWindowByPid,
    getWindowRectByPid,
    getWindowInfoByPid
} = winmgr;

function focusWindowByName(name) {

    if (typeof name === 'string') {
        if (/^\^.+\$$/.test(name)) {
            name = new RegExp(name);
        } else {
            name = new RegExp(`^${name}$`)
        }
    }
    if (name instanceof RegExp) {

    } else {
        throw Error('Error Argument Type');
    }
    const list = getWindowInfoList();

    for (let info of list) {
        if (name.test(info.title)) {
            focusWindowByPid(info.pid);
            return true;
        }
    }

    return false;
}

function getWindowRectByName(name) {

    if (typeof name === 'string') {
        if (/^\^.+/.test(name) || /.+\$$/.test(name)) {
            name = new RegExp(name);
        } else {
            name = new RegExp(`^${name}$`)
        }
    }
    if (name instanceof RegExp) {

    } else {
        throw Error('Error Argument Type');
    }

    const list = getWindowInfoList();

    for (let info of list) {
        if (name.test(info.title)) {
            return getWindowRectByPid(info.pid);
        }
    }

}

function getWindowInfoByName(name) {

    if (typeof name === 'string') {
        if (/^\^.+/.test(name) || /.+\$$/.test(name)) {
            name = new RegExp(name);
        } else {
            name = new RegExp(`^${name}$`)
        }
    }
    if (name instanceof RegExp) {

    } else {
        throw Error('Error Argument Type');
    }

    const list = getWindowInfoList();

    for (let info of list) {
        if (name.test(info.title)) {
            return getWindowInfoByPid(info.pid);
        }
    }

}

module.exports = {
    focusWindowByName,
    getWindowRectByName,
    getWindowInfoByName,
    ...winmgr
}