import { readFile } from 'fs';
import { promisify } from 'util';

const readFileAsync = promisify(readFile);

const idiomDict = [];
const tuneConvertDict = {};
const graph = {}; // graph['wei']['wei'] = [{}, {}, ...]
const nodes = new Set();
const endNodes = new Set();
const beginNodes = new Set();

const tuneDict = {
  a: ['ā', 'á', 'ǎ', 'à'],
  o: ['ō', 'ó', 'ǒ', 'ò'],
  e: ['ē', 'é', 'ě', 'è'],
  i: ['ī', 'í', 'ǐ', 'ì'],
  u: ['ū', 'ú', 'ǔ', 'ù'],
  v: ['ǖ', 'ǘ', 'ǚ', 'ǜ', 'ü'],
  g: ['ɡ'],
};

const parseWord = (pinyin) => {
  const splited = pinyin.split(/[\s,]+/);
  const begin = splited[0]
    .split('')
    .map(e => tuneConvertDict[e] || e)
    .join('');
  const end = splited[splited.length - 1]
    .split('')
    .map(e => tuneConvertDict[e] || e)
    .join('');
  return { begin, end };
};

const init = async () => {
  const jsonData = JSON.parse(await readFileAsync('data/idiom.json'));
  for (const [k, v] of Object.entries(tuneDict)) {
    v.forEach((e) => { tuneConvertDict[e] = k; });
  }
  jsonData.forEach(({ word, pinyin }) => {
    if (word.length === 4) { // for idiot QQ just recognizes four-length idioms
      idiomDict.push({ word, ...parseWord(pinyin) });
    }
  });
};

const initGraph = () => {
  idiomDict.forEach((ele) => {
    const { begin, end } = ele;
    if (!(begin in graph)) graph[begin] = {};
    if (!(end in graph[begin])) graph[begin][end] = [];
    graph[begin][end].push(ele);
    nodes.add(begin);
    nodes.add(end);
    endNodes.add(end);
    beginNodes.add(begin);
  });
};

Array.prototype.chunk = function (chunkSize) {
  return Array(Math.ceil(this.length / chunkSize))
    .fill()
    .map((_, index) => index * chunkSize)
    .map(begin => this.slice(begin, begin + chunkSize));
};

const getIdiom = (arr) => {
  return arr.map(({ word }) => word).sort((a, b) => a.length - b.length)
    .chunk(4)
    .map(e => e.join(' | '))
    .map(e => `\t${e}`)
    .join('\n');
};

const printPath = (info, trace) => {
  for (let i = 0; i < trace.length - 1; ++i) {
    const from = trace[i];
    const to = trace[i + 1];
    console.log(`Step.${i + 1}\t${from} => ${to}`);
    console.log(getIdiom(graph[from][to]));
  }
  console.log(Array(20).fill('=').join(''));
};

const getAllPaths = (info, from, to, stack = [], depth, current = 0) => {
  if (!info[to].pre || current >= depth) return false;
  if (info[to].pre[0] === from && current === depth - 1) {
    const ur = from.startsWith('_') ? from.slice(1) : from;
    printPath(info, [...stack, ur].reverse());
    return true;
  }
  let res = false;
  for (const e of info[to].pre) {
    res = getAllPaths(info, from, e, [...stack, e], depth, current + 1) || res;
  }
  return res;
};

const BFS = (from, to, depth) => {
  const info = {};
  const Q = [];
  for (const e of [...nodes]) {
    info[e] = {
      vis: false,
      pre: null,
      key: e,
    };
  }
  if (from === to) {
    from = `_${from}`;
  }
  info[from] = {
    vis: false,
    pre: null,
    key: from,
  };
  Q.push(from);
  let u;
  while (Q.length > 0) {
    u = Q.shift();
    const ur = u.startsWith('_') ? u.slice(1) : u;
    for (const v of Object.keys(graph[ur] || [])) {
      if (!info[v].vis) {
        info[v].vis = true;
        info[v].pre = [u];
        Q.push(v);
      } else if (!info[v].pre.includes(u)) {
        info[v].pre.push(u);
      }
    }
  }
  return getAllPaths(info, from, to, [to], depth);
};

const task = async (func) => {
  await init();
  initGraph();
  func();
};

const fromTo = (from, to, depth) => {
  return BFS(from, to, depth);
};

const startsWith = (target) => {
  console.log(getIdiom(idiomDict
    .filter(({ begin }) => begin === target)));
};

const endsWith = (target) => {
  console.log(getIdiom(idiomDict
    .filter(({ end }) => end === target)));
};

const cycle = () => {
  console.log(getIdiom(idiomDict
    .filter(({ begin, end }) => begin === end)));
};

task(() => {
  // startsWith('lan');
  // console.log(Array(100).fill('-').join(''));
  // endsWith('lan');
  // console.log(Array(100).fill('-').join(''));
  // cycle();
  // console.log(Array(100).fill('-').join(''));
  // console.log(fromTo('wei', 'fou', 2));
  // console.log(Array(100).fill('-').join(''));
  // console.log(beginNodes.size);
  // console.log(endNodes.size);
  const endArray = [...endNodes];
  const beginArray = [...beginNodes];
  const diff = endArray.filter(x => !beginArray.includes(x));
  // console.log(diff);
  const real = ['ha', 'fou', 'nou', 'niang', 'gei'];
  real.forEach((e) => {
    console.log(`  ${e} : `);
    endsWith(e);
    console.log(Array(20).fill('-').join(''));
  });
  // console.log(nodes.size);
  // let cnt = 0;
  // for (const e of Object.keys(graph)) { cnt += Object.keys(graph[e]).length; }
  // console.log(cnt);
});
