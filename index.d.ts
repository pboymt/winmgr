declare module "winmgr" {

    interface WindowInfo {
        pid: number;
        title: string;
    }

    interface WindowRect {
        x: number;
        y: number;
        width: number;
        height: number;
    }

    function getTitleList(): string[];

    function getProcessIdList(): number[];

    function getWindowInfoList(): WindowInfo[];

    function focusWindowByPid(pid: number): boolean;

    function focusWindowByName(name: string): boolean;

    function getWindowRectByPid(pid: number): WindowRect;

}