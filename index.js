const bindings = require('bindings');

const {
    getTitleList,
    getProcessIdList,
    getWindowInfoList,
    focusWindowByPid
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

module.exports = {
    getTitleList,
    getProcessIdList,
    getWindowInfoList,
    focusWindowByPid,
    focusWindowByName
}