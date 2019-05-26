declare module "winmgr" {

    interface WindowRect {
        x: number;
        y: number;
        width: number;
        height: number;
    }

    interface WindowInfo extends WindowRect {
        pid: number;
        title: string;
    }

    interface ScreenSize {
        width: number;
        height: number;
    }

    function getTitleList(): string[];

    function getProcessIdList(): number[];

    function getWindowInfoList(): WindowInfo[];

    function getWindowInfoByPid(pid: number): WindowInfo;

    function getWindowInfoByName(name: string | RegExp): WindowInfo;

    function focusWindowByPid(pid: number): boolean;

    function focusWindowByName(name: string | RegExp): boolean;

    function getWindowRectByPid(pid: number): WindowRect;

    function getWindowRectByName(name: string | RegExp): WindowRect;

    function getScreenWidth(): number;

    function getScreenHeight(): number;

    function getScreenSize(): ScreenSize;
}