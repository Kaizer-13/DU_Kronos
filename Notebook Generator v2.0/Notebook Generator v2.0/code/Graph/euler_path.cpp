/* 1 -based */
vector <int> ed[MAX+5], sltn;
int inDeg[MAX+5], outDeg[MAX+5];
bool vis[MAX+5];

void dfs(int nd) {
  vis[nd] = true; // used to check the connectivity of the graph
  while(ed[nd].size()) {
    int v = ed[nd].back();
    ed[nd].pop_back();
    dfs(v);
  }
  sltn.pb(nd);
}

// returns 0 if no Euler path or circuit exists
// returns 1 if a Euler trail exists
// returns 2 if a Euler circuit exists
int findEuler (int n) {
  int src , snk , ret = 1;
  bool found_src = false, found_snk = false;
  CLR(inDeg); CLR(outDeg);
  for(int u = 1; u <= n; u++) {
    for(int i = 0; i<ed[u].size(); i++) {
      int v = ed[u][i];
      outDeg[u]++;
      inDeg[v]++;
    }
  }
  int diff;
  for(int i = 1; i<=n; i++) {
    diff = outDeg[i] - inDeg[i];
    if(diff == 1) {
      if(found_src) return 0;
      found_src = true;
      src = i;
    }
    else if (diff == -1) {
      if(found_snk) return 0;
      found_snk = true;
      snk = i;
    }
    else if(diff != 0) return 0;
  }
  if(!found_src) {
    // there actually exists a euler cycle. So you need to pick a random node with non-zero degrees.
    ret = 2;
    for(int i = 1 ; i <= n ; i++) {
      if( outDeg[i] ) {
        found_src = true;
        src = i;
        break;
      }
    }
  }
  if(!found_src) return ret; // every node has out-degree 0
  CLR(vis);
  sltn.clear();
  dfs(src);
  for(int i = 1; i<=n; i++) {
    // the underlying graph is not even weakly connected.
    if(outDeg[i] && !vis[i]) return 0;
  }
  // printing path
  for(int i = (int)sltn.size()-1; i>=0; i--) printf("%d ",sltn[i]);
  puts("");

  return ret;
}

