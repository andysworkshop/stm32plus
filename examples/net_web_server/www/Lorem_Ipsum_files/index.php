// See [[mw:Reference Tooltips]]

window.pg || $(document).ready( function($) {

    // Make sure we are in article, project, or help namespace
    if ( wgCanonicalNamespace === '' || wgCanonicalNamespace === 'Project' || wgCanonicalNamespace === 'Help' ) {
        function toggleRT(o){
            mw.loader.using("jquery.cookie",function(){
                    $.cookie("RTsettings",o+"|"+ settings[1] + "|" + settings[2], {path:"/",expires:90});
                    location.reload();
            })
        }
        var settings = document.cookie.split("RTsettings=")[1];
        settings = settings ? settings.split(";")[0].split("%7C") : [1, 200, +("ontouchstart" in document.documentElement)];
        if( settings[0] == 0 ) {
            var footer = $("#footer-places, #f-list");
            if( footer.length === 0 ) {
                    footer = $("#footer li").parent();
            }
            footer.append($("<li>").append($("<a>").text("Enable Reference Tooltips").attr("href","javascript:(function(){})()").click(function(){toggleRT(1)})));
            return;
        }
        var isTouchscreen = +settings[2],
            timerLength = isTouchscreen ? 0 : +settings[1],
            settingsMenu;
        $(".reference").each( function() {
            var tooltipNode, hideTimer, showTimer, checkFlip = false;
            function findRef( h ){
                    h = h.firstChild.getAttribute("href"); h = h && h.split("#"); h = h && h[1];
                    h = h && document.getElementById( h );
                    h = h && h.nodeName == "LI" && h;
                    return h;
            }
            function hide( refLink ){
                    if( tooltipNode && tooltipNode.parentNode == document.body ) {
                            hideTimer = setTimeout( function() {
                                    $(tooltipNode).animate({opacity: 0}, 100, function(){ document.body.removeChild( tooltipNode ) })
                            }, isTouchscreen ? 16 : 100)
                    } else {
                            var h = findRef( refLink );
                            h && (h.style.border = "");
                    }
            }
            function show(){
                    if( !tooltipNode.parentNode || tooltipNode.parentNode.nodeType === 11 ){
                            document.body.appendChild( tooltipNode );
                            checkFlip = true;
                    }
                    $(tooltipNode).stop().animate({opacity: 1}, 100)
                    clearTimeout( hideTimer );
            }
            function openSettingsMenu(){
                    if( settingsMenu ) {
                            settingsMenu.dialog( "open" );
                    } else {
                            settingsMenu = $("<form>").append(
                                    $("<button>").css("width","100%").text("Disable Reference Tooltips").button().click(function(){toggleRT(0)}),
                                    $("<br>"),
                                    $("<small>").text("Once disabled, Reference Tooltips can be re-enabled using a link in the footer of the page."),
                                    $("<hr>"),
                                    $("<label>").text("Delay before the tooltip appears (in milliseconds): ").append($("<input>").attr({"type":"number","value":settings[1],step:50,min:0,max:5000})),
                                    $("<br>"),
                                    $("<span>").text("Tooltip is activated by:"),
                                    $("<label>").append(
                                            $("<input>").attr({"type":"radio", "name":"RTActivate", "checked":settings[2]==0&&"checked", "disabled":"ontouchstart" in document.documentElement&&"disabled"}),
                                            "hovering"
                                    ),
                                    $("<label>").append(
                                            $("<input>").attr({"type":"radio", "name":"RTActivate", "checked":settings[2]==1&&"checked"}),
                                            "clicking"
                                    )
                            ).submit(function(e){e.preventDefault()}).dialog({modal:true,width:500,title:"Reference Tooltips options",buttons:{"Save settings":function(){
                                    var a = this.getElementsByTagName("input"),
                                            b = +a[0].value;
                                    $.cookie("RTsettings","1|"+ (b > -1 && b < 5001 ? b : settings[1]) + (a[1].checked ? "|0" : "|1"), {path:"/",expires:90});
                                    location.reload();
                            }}});
                    }
            }
            $(this)[ isTouchscreen ? 'click' : 'hover' ](function( e ){
                    var _this = this;
                    if( isTouchscreen ) {
                            e.preventDefault();
                            (tooltipNode && tooltipNode.parentNode == document.body) || setTimeout( function(){
                                    $( document.body ).on("click touchstart", function( e ) {
                                            e = e || event;
                                            e = e.target || e.srcElement;
                                            for( ; e && !$( e ).hasClass( "referencetooltip" ) ; )
                                                    e = e.parentNode;
                                            if( !e ){
                                                    clearTimeout( showTimer );
                                                    hide( _this );
                                                    $(document.body).off("click touchstart", arguments.callee)
                                            }
                                    })
                            }, 0);
                    }
                    showTimer && clearTimeout( showTimer );
                    showTimer = setTimeout( function() {
                            var h = findRef( _this );
                            if( !h ){return};
                            if( !isTouchscreen && ( window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop || 0 ) + $(window).height() > $( h ).offset().top + h.offsetHeight ) {
                                    h.style.border = "#080086 2px solid";
                                    return;
                            }
                            if(!tooltipNode){
                                    tooltipNode = document.createElement("ul");
                                    tooltipNode.className = "referencetooltip";
                                    var c = tooltipNode.appendChild( h.cloneNode( true ) );
                                    try {
                                            if( c.firstChild.nodeName != "A" ) {
                                                    while( c.childNodes[1].nodeName == "A" && c.childNodes[1].getAttribute( "href" ).indexOf("#cite_ref-") !== -1 ) {
                                                            do { c.removeChild( c.childNodes[1] ) } while ( c.childNodes[1].nodeValue == " " );
                                                    }
                                            }
                                    } catch (e) { mw.log(e) }
                                    c.removeChild( c.firstChild );
                                    $( tooltipNode.firstChild.insertBefore( document.createElement( "span" ), tooltipNode.firstChild.firstChild ) ).addClass("RTsettings").attr("title", "Tooltip settings").click(function(){
                                            mw.loader.using(["jquery.cookie","jquery.ui.dialog"], openSettingsMenu);
                                    })
                                    tooltipNode.appendChild( document.createElement( "li" ) );
                                    isTouchscreen || $(tooltipNode).hover(show, hide);
                            }
                            show();
                            var o = $(_this).offset(), oH = tooltipNode.offsetHeight;
                            $(tooltipNode).css({top: o.top - oH, left: o.left - 7 });
                            if( tooltipNode.offsetHeight > oH ) { // is it squished against the right side of the page?
                                    $(tooltipNode).css({left:'auto',right:0});
                                    tooltipNode.lastChild.style.marginLeft = (o.left - tooltipNode.offsetLeft) + "px";
                            }
                            if( checkFlip ) {
                                    if( o.top < tooltipNode.offsetHeight + ( window.pageYOffset || document.documentElement.scrollTop || document.body.scrollTop || 0 ) ) { // is part of it above the top of the screen?
                                            $(tooltipNode).addClass("RTflipped").css({top: o.top + 12});
                                    } else if( tooltipNode.className === "referencetooltip RTflipped" ) { // cancel previous
                                            $(tooltipNode).removeClass("RTflipped");
                                    }
                                    checkFlip = false;
                            }
                    }, timerLength);
            }, isTouchscreen ? undefined : function(){clearTimeout(showTimer); hide(this); } )
 
        } );
        
    }

} );