declare module "winmgr" {

    interface WindowInfo {
        pid: number;
        title: string;
    }

    function getTitleList(): string[];

    function getProcessIdList(): number[];

    function getWindowInfoList(): WindowInfo[];

    function focusWindowByPid(pid: number): boolean;

    function focusWindowByName(name: string): boolean;

}