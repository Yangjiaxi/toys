"use strict";

window.onload = init;

var data = makeData(nameList);
var controller = makeHandle();

function makeData(dataMap) {
    var obj = {
        data: dataMap,
        idList: [],
        total: 0, // change later
        count: 0,
        init: function () {
            this.idList = []
            for (var it in dataMap) {
                this.idList.push(it);
            }
            this.total = this.idList.length;
            this.count = 0;
        },
        hasNext: function () {
            if (this.total === this.count) {
                return false;
            }
            return true;
        },
        next: function () {
            var res = parseInt(Math.random() * (this.total - this.count), 10);
            this.count += 1;
            return res;
        },
        accept: function (idx) {
            this.idList.splice(idx, 1);
        },
        number: function (idx) {
            return this.idList[idx];
        },
        retrieve: function (idx) {
            return this.data[this.number(idx)];
        },
        showAll: function () {
            for (var it in this.idList) {
                console.log(it, this.number(it), this.retrieve(it));
            }
        }
    }
    obj.init();
    return obj;
}

function makeHandle() {

    var obj = {}
    obj.freshList = document.querySelector("#fresh-list");
    obj.freshCounter = document.querySelector("#fresh-counter");
    obj.freshCounter_s = document.querySelector("#fresh-counter-small");

    obj.expiredList = document.querySelector("#expired-list");
    obj.expiredCounter = document.querySelector("#expired-counter");
    obj.expiredCounter_s = document.querySelector("#expired-counter-small");

    obj.centerName = document.querySelector("#lucky-name");

    obj.buttonNext = document.querySelector("#generate-next");
    obj.buttonRst = document.querySelector("#restart");
    obj.buttonContact = document.querySelector("#contact");

    obj.addItemToList = function (listObj, item) {
        var node = document.createElement("li");
        var textNode = document.createTextNode(item);
        node.appendChild(textNode);
        listObj.appendChild(node);
    };
    obj.reloadFresh = function () {
        for (var it in data.idList) {
            var idx = data.number(it);
            obj.addItemToList(obj.freshList, idx + ". " + data.retrieve(it));
        }
    };
    obj.reloadCounter = function () {
        obj.freshCounter.innerHTML = obj.freshCounter_s.innerHTML = data.total - data.count;
        obj.expiredCounter.innerHTML = obj.expiredCounter_s.innerHTML = data.count;
    };
    obj.changeCenter = function (text) {
        obj.centerName.innerHTML = text;
    };
    obj.hasNext = function () {
        return data.hasNext();
    };
    obj.next = function () {
        if (obj.hasNext()) {
            var get = data.next();
            var target = obj.freshList.children[get];
            obj.freshList.removeChild(target);
            obj.changeCenter(data.retrieve(get));
            obj.addItemToList(obj.expiredList, data.count + ". " + data.retrieve(get));
            data.accept(get);
            obj.reloadCounter();
        }
        else {
            alert("已抽完，请重新开始！")
        }
    };
    obj.restart = function () {
        if (confirm("重新开始？")) {
            obj.expiredList.innerHTML = "";
            obj.freshList.innerHTML = "";
            data.init();
            obj.reloadFresh();
            obj.changeCenter("Turn?");
            obj.reloadCounter();
            obj.buttonNext.focus();
        }
    };
    obj.init = function () {
        obj.reloadFresh();
        obj.reloadCounter();
        obj.buttonNext.addEventListener("click", obj.next);
        obj.buttonRst.addEventListener("click", obj.restart);
        obj.buttonContact.addEventListener("click", function () { alert("Please email to 1468764755@qq.com.") });
    };
    obj.init();
    return obj;
}

function init() {
    console.log("Loaded!");
    controller.buttonNext.focus();
}
