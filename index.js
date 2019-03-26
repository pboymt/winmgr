const bindings = require('bindings');

const {
    getTitleList,
    getProcessIdList,
    getWindowInfoList,
    focusWindowByPid,
    getWindowRectByPid
} = bindings('winmgr');

function focusWindowByName(name) {

    if (!name || typeof name !== 'string') {
        throw Error('Error Argument Type');
    }
    const list = getWindowInfoList();

    for (let info of list) {
        if (String(info.title).includes(name)) {
            focusWindowByPid(info.pid);
            return true;
        }
    }

    return false;
}

function getWindowRectByName(name) {

    if (!name || typeof name !== 'string') {
        throw Error('Error Argument Type');
    }
    const list = getWindowInfoList();

    for (let info of list) {
        if (String(info.title).includes(name)) {
            return getWindowRectByPid(info.pid);
        }
    }

}

module.exports = {
    getTitleList,
    getProcessIdList,
    getWindowInfoList,
    focusWindowByPid,
    focusWindowByName,
    getWindowRectByPid,
    getWindowRectByName
}