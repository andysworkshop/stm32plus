mw.loader.implement("ext.vector.collapsibleNav",function(){(function(mw,$){'use strict';var map,version;function toggle($element){$.cookie('vector-nav-'+$element.parent().attr('id'),$element.parent().is('.collapsed'),{'expires':30,'path':'/'});$element.parent().toggleClass('expanded').toggleClass('collapsed').find('.body').slideToggle('fast');}map={ltr:{opera:[['>=',9.6]],konqueror:[['>=',4.0]],blackberry:false,ipod:false,iphone:false,ps3:false},rtl:{opera:[['>=',9.6]],konqueror:[['>=',4.0]],blackberry:false,ipod:false,iphone:false,ps3:false}};if(!$.client.test(map)){return true;}version=1;if(mw.config.get('wgCollapsibleNavForceNewVersion')){version=2;}else{if(mw.config.get('wgCollapsibleNavBucketTest')){version=$.cookie('vector-nav-pref-version');if(version===null){version=Math.round(Math.random()+1);$.cookie('vector-nav-pref-version',version,{expires:30,path:'/'});}}}$(function($){var limit,threshold,languages,acceptLangCookie,$primary,$secondary,i,$link,count,$headings,tabIndex;if(
version===2){limit=5;threshold=3;$('#p-lang ul').addClass('secondary').before('<ul class="primary"></ul>');languages=['en','fr','de','es','pt','it','ru','ja','nl','pl','zh','sv','ar','tr','uk','fi','no','ca','ro','hu','ksh','id','he','cs','vi','ko','sr','fa','da','eo','sk','th','lt','vo','bg','sl','hr','hi','et','mk','simple','new','ms','nn','gl','el','eu','ka','tl','bn','lv','ml','bs','te','la','az','sh','war','br','is','mr','be-x-old','sq','cy','lb','ta','zh-classical','an','jv','ht','oc','bpy','ceb','ur','zh-yue','pms','scn','be','roa-rup','qu','af','sw','nds','fy','lmo','wa','ku','hy','su','yi','io','os','ga','ast','nap','vec','gu','cv','bat-smg','kn','uz','zh-min-nan','si','als','yo','li','gan','arz','sah','tt','bar','gd','tg','kk','pam','hsb','roa-tara','nah','mn','vls','gv','mi','am','ia','co','ne','fo','nds-nl','glk','mt','ang','wuu','dv','km','sco','bcl','mg','my','diq','tk','szl','ug','fiu-vro','sc','rm','nrm','ps','nv','hif','bo','se','sa','pnb','map-bms','lad','lij','crh',
'fur','kw','to','pa','jbo','ba','ilo','csb','wo','xal','krc','ckb','pag','ln','frp','mzn','ce','nov','kv','eml','gn','ky','pdc','lo','haw','mhr','dsb','stq','tpi','arc','hak','ie','so','bh','ext','mwl','sd','ig','myv','ay','iu','na','cu','pi','kl','ty','lbe','ab','got','sm','as','mo','ee','zea','av','ace','kg','bm','cdo','cbk-zam','kab','om','chr','pap','udm','ks','zu','rmy','cr','ch','st','ik','mdf','kaa','aa','fj','srn','tet','or','pnt','bug','ss','ts','pcd','pih','za','sg','lg','bxr','xh','ak','ha','bi','ve','tn','ff','dz','ti','ki','ny','rw','chy','tw','sn','tum','ng','rn','mh','ii','cho','hz','kr','ho','mus','kj'];acceptLangCookie=$.cookie('accept-language');if(acceptLangCookie!==null){if(acceptLangCookie!==''){languages=acceptLangCookie.split(',').concat(languages);}}else{$.getJSON(mw.util.wikiScript('api'),'format=json&action=query&meta=userinfo&uiprop=acceptlang',function(data){var langs=[],j,len,lang;if(data.query&&data.query.userinfo&&data.query.userinfo.acceptlang!==
undefined){for(j=0,lang=data.query.userinfo.acceptlang,len=lang.length;j<len;j++){if(lang[j].q!==0){langs.push(lang[j]['*']);}}}$.cookie('accept-language',langs.join(','),{path:'/',expires:30});});}$primary=$('#p-lang ul.primary');$secondary=$('#p-lang ul.secondary');if($secondary.children().length<limit+threshold){limit+=threshold;}count=0;for(i=0;i<languages.length;i++){$link=$secondary.find('.interwiki-'+languages[i]);if($link.length){if(count++<limit){$link.appendTo($primary);}else{break;}}}if(count<limit){$secondary.children().each(function(){if(count++<limit){$(this).appendTo($primary);}else{return false;}});}if($secondary.children().length===0){$secondary.remove();}else{$('#p-lang').after('<div id="p-lang-more" class="portal"><h3></h3><div class="body"></div></div>');$('#p-lang-more h3').text(mw.msg('vector-collapsiblenav-more'));$secondary.appendTo($('#p-lang-more .body'));}$('#p-lang').addClass('persistent');}$('#mw-panel > .portal:first').addClass('first persistent');$(
'#mw-panel').addClass('collapsible-nav');$('#mw-panel > .portal:not(.persistent)').each(function(i){var id=$(this).attr('id'),state=$.cookie('vector-nav-'+id);$(this).find('h3, h5').wrapInner($('<a href="#"></a>').click(false));if(state==='true'||(state===null&&i<1)||(state===null&&version===1&&id==='p-lang')){$(this).addClass('expanded').removeClass('collapsed').find('.body').hide().show();}else{$(this).addClass('collapsed').removeClass('expanded');}if(state!==null){$.cookie('vector-nav-'+$(this).attr('id'),state,{'expires':30,'path':'/'});}});$headings=$('#mw-panel > .portal:not(.persistent) > h3, #mw-panel > .portal:not(.persistent) > h5');tabIndex=$(document).lastTabIndex()+1;$('#searchInput').attr('tabindex',tabIndex++);$headings.attr('tabindex',function(){return tabIndex++;});$('#mw-panel').delegate('.portal:not(.persistent) > h3, .portal:not(.persistent) > h5','keydown',function(e){if(e.which===13||e.which===32){toggle($(this));}}).delegate(
'.portal:not(.persistent) > h3, .portal:not(.persistent) > h5','mousedown',function(e){if(e.which!==3){toggle($(this));$(this).blur();}return false;});});}(mediaWiki,jQuery));;},{"css":[
"#mw-panel.collapsible-nav .portal{background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIwAAAABCAMAAAA7MLYKAAAAS1BMVEXb29vy8vLv7+/c3NzZ2dni4uLr6+vt7e3s7Ozw8PDn5+fj4+Ph4eHd3d3f39/o6Ojl5eXp6enx8fHa2trg4ODq6urk5OTz8/PY2NjolWftAAAAO0lEQVR4XrXAhwGAMAgEQB5I71X3n9QpPHqAGZidt2e02G8yedciQkv1/YPqIpFSdzbp9tjGsd4xhwl8yuMKHhkJhm8AAAAASUVORK5CYII=) left top no-repeat;background:url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/images/portal-break.png?2013-06-20T16:36:40Z) left top no-repeat!ie;padding:0.25em 0 !important;margin:-11px 9px 10px 11px}#mw-panel.collapsible-nav .portal h3,#mw-panel.collapsible-nav .portal h5{color:#4D4D4D;font-weight:normal;background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQBAMAAADt3eJSAAAAD1BMVEX////d3d2ampqxsbF5eXmCtCYvAAAAAXRSTlMAQObYZgAAADBJREFUeF6dzNEJACAMA1HdINQJCp1Ebv+ZlLYLaD4f4cbnDNi6MAO8KCHJ+7X02j3mzgMQe93HcQAAAABJRU5ErkJggg==) left center no-repeat;background:url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/images/open.png?2013-06-20T16:36:40Z) left center no-repeat!ie; background-image:-webkit-linear-gradient(transparent,transparent),url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhLS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoKPHN2ZwogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iCiAgIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiCiAgIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIKICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIgogICB2ZXJzaW9uPSIxLjEiCiAgIHdpZHRoPSIxNiIKICAgaGVpZ2h0PSIxNiIKICAgaWQ9InN2ZzIiPgogIDxkZWZzCiAgICAgaWQ9ImRlZnM0IiAvPgogIDxtZXRhZGF0YQogICAgIGlkPSJtZXRhZGF0YTciPgogICAgPHJkZjpSREY+CiAgICAgIDxjYzpXb3JrCiAgICAgICAgIHJkZjphYm91dD0iIj4KICAgICAgICA8ZGM6Zm9ybWF0PmltYWdlL3N2Zyt4bWw8L2RjOmZvcm1hdD4KICAgICAgICA8ZGM6dHlwZQogICAgICAgICAgIHJkZjpyZXNvdXJjZT0iaHR0cDovL3B1cmwub3JnL2RjL2RjbWl0eXBlL1N0aWxsSW1hZ2UiIC8+CiAgICAgICAgPGRjOnRpdGxlPjwvZGM6dGl0bGU+CiAgICAgIDwvY2M6V29yaz4KICAgIDwvcmRmOlJERj4KICA8L21ldGFkYXRhPgogIDxnCiAgICAgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwtMTAzNi4zNjIyKSIKICAgICBpZD0ibGF5ZXIxIj4KICAgIDxwYXRoCiAgICAgICBkPSJNIDEwLjAyODYyNCwzLjc3Mjk5MzIgOC4xOTc2NDQyLDYuOTQ0MzQyNCA2LjM2NjY2NDksMy43NzI5OTMyIHoiCiAgICAgICB0cmFuc2Zvcm09Im1hdHJpeCgyLjczMDc3OTEsMCwwLDEuNTc2NjE2LC0xNC4zODU5NTYsMTAzNi40MTM2KSIKICAgICAgIGlkPSJwYXRoMjk4NSIKICAgICAgIHN0eWxlPSJmaWxsOiM3OTc5Nzk7ZmlsbC1vcGFjaXR5OjE7c3Ryb2tlOm5vbmUiIC8+CiAgPC9nPgo8L3N2Zz4K);background-image:-webkit-linear-gradient(transparent,transparent),url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/images/open.svg?2013-06-20T16:36:40Z)!ie;background-image:linear-gradient(transparent,transparent),url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhLS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoKPHN2ZwogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iCiAgIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiCiAgIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIKICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIgogICB2ZXJzaW9uPSIxLjEiCiAgIHdpZHRoPSIxNiIKICAgaGVpZ2h0PSIxNiIKICAgaWQ9InN2ZzIiPgogIDxkZWZzCiAgICAgaWQ9ImRlZnM0IiAvPgogIDxtZXRhZGF0YQogICAgIGlkPSJtZXRhZGF0YTciPgogICAgPHJkZjpSREY+CiAgICAgIDxjYzpXb3JrCiAgICAgICAgIHJkZjphYm91dD0iIj4KICAgICAgICA8ZGM6Zm9ybWF0PmltYWdlL3N2Zyt4bWw8L2RjOmZvcm1hdD4KICAgICAgICA8ZGM6dHlwZQogICAgICAgICAgIHJkZjpyZXNvdXJjZT0iaHR0cDovL3B1cmwub3JnL2RjL2RjbWl0eXBlL1N0aWxsSW1hZ2UiIC8+CiAgICAgICAgPGRjOnRpdGxlPjwvZGM6dGl0bGU+CiAgICAgIDwvY2M6V29yaz4KICAgIDwvcmRmOlJERj4KICA8L21ldGFkYXRhPgogIDxnCiAgICAgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwtMTAzNi4zNjIyKSIKICAgICBpZD0ibGF5ZXIxIj4KICAgIDxwYXRoCiAgICAgICBkPSJNIDEwLjAyODYyNCwzLjc3Mjk5MzIgOC4xOTc2NDQyLDYuOTQ0MzQyNCA2LjM2NjY2NDksMy43NzI5OTMyIHoiCiAgICAgICB0cmFuc2Zvcm09Im1hdHJpeCgyLjczMDc3OTEsMCwwLDEuNTc2NjE2LC0xNC4zODU5NTYsMTAzNi40MTM2KSIKICAgICAgIGlkPSJwYXRoMjk4NSIKICAgICAgIHN0eWxlPSJmaWxsOiM3OTc5Nzk7ZmlsbC1vcGFjaXR5OjE7c3Ryb2tlOm5vbmUiIC8+CiAgPC9nPgo8L3N2Zz4K);background-image:linear-gradient(transparent,transparent),url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/images/open.svg?2013-06-20T16:36:40Z)!ie;padding:4px 0 3px 1.5em;margin-bottom:0}#mw-panel.collapsible-nav .portal h3:hover,#mw-panel.collapsible-nav .portal h5:hover{cursor:pointer;text-decoration:none}#mw-panel.collapsible-nav .portal h3 a,#mw-panel.collapsible-nav .portal h5 a{color:#4D4D4D;text-decoration:none}#mw-panel.collapsible-nav .portal .body{background:none !important;padding-top:0;display:none}#mw-panel.collapsible-nav .portal .body ul li{padding:0.25em 0} #mw-panel.collapsible-nav .portal.first h3,#mw-panel.collapsible-nav .portal.first h5{display:none}#mw-panel.collapsible-nav .portal.first{background-image:none;margin-top:0} #mw-panel.collapsible-nav .portal.persistent .body{display:block}#mw-panel.collapsible-nav .portal.persistent h3,#mw-panel.collapsible-nav .portal.persistent h5{background:none !important;padding-left:0.7em;cursor:default}#mw-panel.collapsible-nav .portal.persistent .body{margin-left:0.5em} #mw-panel.collapsible-nav .portal.collapsed h3,#mw-panel.collapsible-nav .portal.collapsed h5{color:#0645AD;background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQAgMAAABinRfyAAAADFBMVEX///95eXnd3d2dnZ3aAo3QAAAAAXRSTlMAQObYZgAAADFJREFUeF5dyzEKACAMA0CXolNe2Id09Kl5igZahWY4AiGjZwmIuS9GEcJfY63Ix88Bol4EYP1O7JMAAAAASUVORK5CYII=) left center no-repeat;background:url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/images/closed-ltr.png?2013-06-20T16:36:40Z) left center no-repeat!ie; background-image:-webkit-linear-gradient(transparent,transparent),url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhLS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoKPHN2ZwogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iCiAgIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiCiAgIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIKICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIgogICB2ZXJzaW9uPSIxLjEiCiAgIHdpZHRoPSIxNiIKICAgaGVpZ2h0PSIxNiIKICAgaWQ9InN2ZzIiPgogIDxkZWZzCiAgICAgaWQ9ImRlZnM0IiAvPgogIDxtZXRhZGF0YQogICAgIGlkPSJtZXRhZGF0YTciPgogICAgPHJkZjpSREY+CiAgICAgIDxjYzpXb3JrCiAgICAgICAgIHJkZjphYm91dD0iIj4KICAgICAgICA8ZGM6Zm9ybWF0PmltYWdlL3N2Zyt4bWw8L2RjOmZvcm1hdD4KICAgICAgICA8ZGM6dHlwZQogICAgICAgICAgIHJkZjpyZXNvdXJjZT0iaHR0cDovL3B1cmwub3JnL2RjL2RjbWl0eXBlL1N0aWxsSW1hZ2UiIC8+CiAgICAgICAgPGRjOnRpdGxlPjwvZGM6dGl0bGU+CiAgICAgIDwvY2M6V29yaz4KICAgIDwvcmRmOlJERj4KICA8L21ldGFkYXRhPgogIDxnCiAgICAgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwtMTAzNi4wMjg4KSIKICAgICBpZD0ibGF5ZXIxIj4KICAgIDxwYXRoCiAgICAgICBkPSJNIDEwLjAyODYyNCwzLjc3Mjk5MzIgOC4xOTc2NDQyLDYuOTQ0MzQyNCA2LjM2NjY2NDksMy43NzI5OTMyIHoiCiAgICAgICB0cmFuc2Zvcm09Im1hdHJpeCgwLC0yLjczMDc3OTEsMS41NzY2MTYsMCwwLjA1MTQzODU1LDEwNjYuNDE0OCkiCiAgICAgICBpZD0icGF0aDI5ODUiCiAgICAgICBzdHlsZT0iZmlsbDojNzk3OTc5O2ZpbGwtb3BhY2l0eToxO3N0cm9rZTpub25lIiAvPgogIDwvZz4KPC9zdmc+Cg==);background-image:-webkit-linear-gradient(transparent,transparent),url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/./images/closed-ltr.svg?2013-06-20T16:36:40Z)!ie;background-image:linear-gradient(transparent,transparent),url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjwhLS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoKPHN2ZwogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iCiAgIHhtbG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiCiAgIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIKICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIgogICB2ZXJzaW9uPSIxLjEiCiAgIHdpZHRoPSIxNiIKICAgaGVpZ2h0PSIxNiIKICAgaWQ9InN2ZzIiPgogIDxkZWZzCiAgICAgaWQ9ImRlZnM0IiAvPgogIDxtZXRhZGF0YQogICAgIGlkPSJtZXRhZGF0YTciPgogICAgPHJkZjpSREY+CiAgICAgIDxjYzpXb3JrCiAgICAgICAgIHJkZjphYm91dD0iIj4KICAgICAgICA8ZGM6Zm9ybWF0PmltYWdlL3N2Zyt4bWw8L2RjOmZvcm1hdD4KICAgICAgICA8ZGM6dHlwZQogICAgICAgICAgIHJkZjpyZXNvdXJjZT0iaHR0cDovL3B1cmwub3JnL2RjL2RjbWl0eXBlL1N0aWxsSW1hZ2UiIC8+CiAgICAgICAgPGRjOnRpdGxlPjwvZGM6dGl0bGU+CiAgICAgIDwvY2M6V29yaz4KICAgIDwvcmRmOlJERj4KICA8L21ldGFkYXRhPgogIDxnCiAgICAgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoMCwtMTAzNi4wMjg4KSIKICAgICBpZD0ibGF5ZXIxIj4KICAgIDxwYXRoCiAgICAgICBkPSJNIDEwLjAyODYyNCwzLjc3Mjk5MzIgOC4xOTc2NDQyLDYuOTQ0MzQyNCA2LjM2NjY2NDksMy43NzI5OTMyIHoiCiAgICAgICB0cmFuc2Zvcm09Im1hdHJpeCgwLC0yLjczMDc3OTEsMS41NzY2MTYsMCwwLjA1MTQzODU1LDEwNjYuNDE0OCkiCiAgICAgICBpZD0icGF0aDI5ODUiCiAgICAgICBzdHlsZT0iZmlsbDojNzk3OTc5O2ZpbGwtb3BhY2l0eToxO3N0cm9rZTpub25lIiAvPgogIDwvZz4KPC9zdmc+Cg==);background-image:linear-gradient(transparent,transparent),url(//bits.wikimedia.org/static-1.22wmf8/extensions/Vector/modules/./images/closed-ltr.svg?2013-06-20T16:36:40Z)!ie;margin-bottom:0}#mw-panel.collapsible-nav .portal.collapsed h3 a,#mw-panel.collapsible-nav .portal.collapsed h5 a{color:#0645AD}#mw-panel.collapsible-nav .portal.collapsed h3:hover,#mw-panel.collapsible-nav .portal.collapsed h5:hover{text-decoration:underline}\n/* cache key: enwiki:resourceloader:filter:minify-css:7:aa662c29012406b81f82cd8f718719a9 */"
]},{"vector-collapsiblenav-more":"More languages"});
/* cache key: enwiki:resourceloader:filter:minify-js:7:3d3774f1d64b2a448e8722fb1590dfdd */