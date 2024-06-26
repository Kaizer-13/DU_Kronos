void calcSubSize(int s,int p) {
  sub[s] = 1;
  for(int x : G[s]) {
    if(x==p) continue;
    calcSubSize(x,s);
    sub[s] += sub[x];
  }
}
void add(int s,int p,int v,int bigchild = -1) {
  freq[color[s]] += v;
  for(int x : G[s]) {
    if(x==p || x==bigchild) continue;
    add(x,s,v);
  }
}
void dfs(int s,int p,bool keep) {
  int bigChild = -1;
  for(int x : G[s]) {
    if(x==p) continue;
    if(bigChild==-1 || sub[bigChild] < sub[x] ) bigChild = x;
  }
  for(int x : G[s]) {
    if(x==p || x==bigChild) continue;
    dfs(x,s,0);
  }
  if(bigChild!=-1) dfs(bigChild,s,1);
  add(s,p,1,bigChild);
  if(keep==0)
    add(s,p,-1);
}

