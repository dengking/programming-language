// wiki.js - included in all Wiki pages.
//
// Must get stored in "/themes/wiki_themes/isocpp/javascript/wiki.js".
// It gets loaded at the bottom of each Wiki/FAQ page - the Markdown machinery automatically inserts:
//     <script src="/themes/wiki_themes/isocpp/javascript/wiki.js"></script>
//
// History:
//   2013-01-15: Marshall Cline: created.
//   2013-01-16: Marshall Cline: now patches the page's <title> and <h2> elements.
//   2013-01-21: Marshall Cline: changed FAQ target-syntax from "### <!-- FAQ: id --> Title" to "### Title {#id}".
//   2013-01-22: Marshall Cline: now has a permalink <a> and <img> after each FAQ.
//   2013-01-23: Marshall Cline: now has tighter constraints on <div class='prettify'> and uses jQuery-style.
//   2013-01-24: Marshall Cline: now patches the page's breadcrumbs area in page's header.
//   2013-01-25: Marshall Cline: fixed two bugs; reorganized for maintainability.
//   2013-02-04: Marshall Cline: changed section-title from <!--...--> to <h6>.
//   2013-02-05: Marshall Cline: fixed bug - jQuery fails with 'h3[id!=""]'
//   2013-02-08: Marshall Cline: added functionality to consistently show permalinks to end-users.
//   2013-02-11: Marshall Cline: changed the section-title from <h6> to <h2>
//   2013-02-13: Marshall Cline: added functionality to change the address-bar to the permalink.
//   2013-02-23: Marshall Cline: removed all functionality related to permalinks.
//   2013-02-25: Marshall Cline: added "version 5" of TOC-plasticity (redirect stale #fragments using info in 'id-defns').
//   2013-02-26: Marshall Cline: added "onhashchange" handler to finish the v5 TOC-plasticity, and moved the "FAQ" prefix into CSS.
//   2013-05-11: Jeremy Kratz: added handlers for retrieving Markdown content on Article Edit page
//   2013-12-15: Jeremy Kratz: added FAQ suggestion popup window handler
//   2013-12-16: Jeremy Kratz: added FAQ suggestion popup to article subsections

$(document).ready(function() {
  "use strict";

  // $pageTitle is the <h2 id="SECTION_ID">SECTION_TITLE</h2> at the *VERY TOP*; it provides the page's ID and title.
  // $faqTitles are the <h3 id="FAQ_ID">FAQ_TITLE</h3> at the top of each FAQ; each provides the FAQ's ID and title.
  var ID_DEFNS_ID      = 'id-defns',
      ID_DEFNS_URL     = '/wiki/faq/' + ID_DEFNS_ID + '/edit',
      IMAGE_DIR        = '/themes/wiki_themes/isocpp/images/',
      MSGBOX_URGENCIES = ['good', 'info', 'warn', 'fail'],
      $pageTitle       = $('#wiki-content div.prettify > h2[id]:first-child'),
      $faqTitles       = $('#wiki-content div.prettify > h3[id]'),  // not 'h3[id!=""]' - jQuery fails with that
      $permalinkIcon   = $('<span/>').html("\xB6"),    // or .html('&para;')  or .append($('<img/>', {src: IMAGE_DIR+'permalink-24x24.png', alt: '&para;'}))
      $modifyIcon      = $('<span/>').html("\u0394");  // or .html('&Delta;') or .append($('<img/>', {src: IMAGE_DIR+'modify-24x24.png', alt: '&Delta;'}))

  // Precreate $('#msgbox-good'), $('#msgbox-info'), $('#msgbox-warn'), and $('#msgbox-fail'):
  $.each(MSGBOX_URGENCIES, function(i, urgency) {
    $('<div/>', {id: 'msgbox-' + urgency})
      .addClass(urgency + '-icon')
      .addClass(urgency + '-banner')
      .css({position: 'fixed', top: '40%', left: '40%', width: '30%', height: '130px', 'overflow-y': 'auto'})
      .hide()
      .mouseover(function() {
        $(this)
          .stop(true, true)
          .show()
          .delay(2000)
          .fadeOut(4000);
      })
      .prependTo('#wiki-content div.prettify');
  });

  // Usage: $msgbox('good').text('Good job!').mouseover();
  // Pre: parameter 'urgency' must be one of MSGBOX_URGENCIES.
  // Effect: hides & clears/empties ALL msgboxes (in case any are unhidden and/or in an animation),
  // Returns: the desired msgbox as a jQuery object.
  // Note: you the caller(!) must (if you want to) populate and unhide the resulting msgbox.
  // Note: you may call 'mouseover()' on the result to unhide it and start a fade-out animation.
  var $msgbox = function(urgency) {
    $.each(MSGBOX_URGENCIES, function(i, urgencyX) {
      $('#msgbox-' + urgencyX)
        .stop(true, true)
        .hide()
        .empty();
    });
    return $('#msgbox-' + urgency);
  };

  // Returns a truthy value (a non-null human-readable error-message) if 'id' is an ill-formed FAQ/section ID.
  // Returns a falsy value (for now, happens to be null) if 'id' is a well-formed FAQ/section ID.
  var idIsIllFormed = function(id) {
    if (!id) {
      return 'ID is missing';
    }
    var problems = id.match(/(?:^[^a-z]|[^a-z0-9\-]+|--+|-$)/g);
    return problems
      ? 'ID "' + id + '" is ill-formed at "' + problems.join('" and "') + '"'
      : null;
  };

  // Sends a message to the site's Admins.
  // Parameter 'data' must be an object with these keys:
  //   "severity": severity level 1-3 (1 is highest severity).
  //   "category": dash-separated identifier giving the category of the message.
  //   "itemId":   the ID of the section/FAQ most closely associated with the issue.
  //   "pathname": the pathname portion of the URI most closely associated with the issue.
  //   "details" : human readable details about the problem.
  var messageToSiteAdmins = function(data) {
    // TODO: change this implementation to a $.post() to a custom URI.

    // If there is an existing banner, stop its animation and remove it:
    $('#message-to-site-admins')
      .stop(true, true)
      .hide()
      .remove();

    // Create, populate, and insert the banner:
    var urgency = 'good';
    $('<div/>')
      .attr('id', 'message-to-site-admins')
      .addClass(urgency + '-icon')
      .addClass(urgency + '-banner')
      .css({position: 'fixed', top: '5px', right: '5px'})
      .mouseover(function() {
        $(this)
          .stop(true, true)
          .show()
          .delay(2000)
          .fadeOut(4000);
      })
      .append('NOTE: this pop-up is just a placeholder to be replaced<br>',
              'by a POST that (silently) sends this to the site Admins:<br><br>',
              $('<table/>').append($('<tr/>').append($('<td/>').text('Severity'), $('<td/>').text(data.severity)),
                                   $('<tr/>').append($('<td/>').text('Category'), $('<td/>').text(data.category)),
                                   $('<tr/>').append($('<td/>').text('Item ID'),  $('<td/>').text(data.itemId)),
                                   $('<tr/>').append($('<td/>').text('Pathname'), $('<td/>').text(data.pathname)),
                                   $('<tr/>').append($('<td/>').text('Details'),  $('<td/>').text(data.details))),
              $('<br>'),
              $('<em/>').text('(Move your mouse over this to keep it visible)'))
      .prependTo('#wiki-content div.prettify')
      .mouseover();
  };

  if ($pageTitle.length) {
    var sectionId = $pageTitle.attr('id');
    if (idIsIllFormed(sectionId)) {
      $('<span/>', {'class': 'warn-banner'})
        .text('Section/page ' + idIsIllFormed(sectionId))
        .insertBefore($pageTitle);
      return;  // don't do anything else if the Markdown author screwed up the page-ID
    }
    var pathname = window.location.pathname;
    if (pathname.slice(-1) === '/') {
      pathname = pathname.substring(0, pathname.length - 1);
    }
    if (sectionId !== 'index' && pathname.replace(/^.*\//, '').toLowerCase() !== sectionId) {
      $('<span/>', {'class': 'warn-banner'})
        .text('Page-name ("' + pathname.replace(/^.*\//, '') + '") mismatches section-ID ("' + sectionId + '")')
        .insertBefore($pageTitle);
      return;  // don't do anything else if the Markdown author screwed up the page-name
    }
    // Use the $pageTitle to patch the page's <title>, the page's 'real' <h2>, and the page's breadcrumb:
    $('title:first')
      .html($pageTitle.html() + (sectionId === 'index' ? '' : ', C++ FAQ'));  // TODO: change to document.title = ...; ?
    $('.breadcrumbs:first')
      .contents()
      .last()
      .wrap('<p/>')
      .parent()
      .html($pageTitle.html())
      .contents()
      .unwrap();
    $('h2:first')
      .empty()
      .addClass('section-title')
      .addClass('has-hover-pop-out')
      .append($pageTitle.html(),
              '&nbsp;',
              $('<a/>', {'class': 'hover-pop-out', href: window.location.pathname, title: 'Permalink to this Section'})
                .append($permalinkIcon.clone()),
              '&nbsp;',
              $('<a/>', {'class': 'hover-pop-out js-inline-popup', href: '#', title: 'Recommend an improvement to this Section', 'data-tooltip': '#popup-suggest'})
                .append($modifyIcon.clone())
                .click(function() {
                  $('#popup-suggest-item-id').val('Item ID: ' + sectionId);
                  $('#popup-suggest-url').val('URL: ' + window.location.protocol + '//' + window.location.host + window.location.pathname);
                })
              );
  }

  // Prepend the TOC at the top of the main part of the page:
  //   <div id="section-toc">
  //       <h4>Contents of this section:</h4>
  //       <ul>
  //           <li> <a href="#FAQ1_ID" title="FAQ1_TITLE_TEXT"> FAQ1_TITLE_HTML </a> </li>
  //           <li> <a href="#FAQ2_ID" title="FAQ2_TITLE_TEXT"> FAQ2_TITLE_HTML </a> </li>
  //           <li> <a href="#FAQ3_ID" title="FAQ3_TITLE_TEXT"> FAQ3_TITLE_HTML </a> </li>
  //           ...etc...
  //       </ul>
  //   </div>
  if (/* $pageTitle.length && */ $faqTitles.length) {
    $('<div/>', {id: 'section-toc'})
      .append($('<h4/>').text('Contents of this section:'),
              $('<ul/>'))
      .insertAfter($pageTitle);
    $faqTitles.each(function() {
      $('<li/>')
        .append($('<a/>', {href: '#' + this.id, title: $(this).text().replace(/\s+/g, ' ')}).html($(this).html()))
        .appendTo($('#section-toc > ul'));
    });
  }

  // Change FAQ-title from:
  //   <h3 id="FAQ_ID">
  //       FAQ_TITLE
  //   </h3>
  // To:
  //   <h3 id="FAQ_ID" class="faq-title has-hover-pop-out">
  //       FAQ_TITLE
  //       &nbsp;
  //       <a class="hover-pop-out" href="#FAQ_ID" title="Permalink to this FAQ"> &para; </a>
  //       &nbsp;
  //       <a class="hover-pop-out" href="javascript:void(0)" title="Recommend an improvement to this FAQ" onclick="..."> &Delta; </a>
  //   </h3>
  // TODO: add link="bookmark" as an attribute of $postLink?
  if (/* $pageTitle.length && */ $faqTitles.length) {
    $faqTitles.each(function() {
      var faqId = this.id;  // for use in closures
      var faqTitle = $(this).text();  // original title before we change it
      $(this)
        .addClass('faq-title')
        .addClass('has-hover-pop-out')
        .append('&nbsp;',
                $('<a/>', {'class': 'hover-pop-out', href: '#' + faqId, title: 'Permalink to this FAQ'})
                  .append($permalinkIcon.clone()),
                '&nbsp;',
                $('<a/>', {'class': 'hover-pop-out js-inline-popup', href: '#', title: 'Recommend an improvement to this FAQ', 'data-tooltip': '#popup-suggest'})
                  .append($modifyIcon.clone())
                  .click(function() {
                    $('#popup-suggest-item-id').val('Item ID: ' + faqId);
                    $('#popup-suggest-url').val('URL: ' + window.location.protocol + '//' + window.location.host + window.location.pathname + '#' + faqId);
                  })
                );
      });
  }

  // Implement permalinks: discover the correct section for a given FAQ ID, and redirect to that page.
  // E.g., after changing the FAQ's TOC, previously saved bookmarks are stale.
  // E.g., after changing the FAQ's TOC, previously shared/posted URLs are stale.
  // E.g., after changing the FAQ's TOC, search engines produce stale links until re-indexing the site.
  // In all cases, we detect the problem, discover the correct URL, and redirect the user to that URL.
  window.onhashchange = function() {
    // Step 1: Check if the URL has a *hash*, i.e., `#faq-id`.
    // If no hash, we succeeded at loading the correct section (not FAQ);
    // Stop successfully.
    if (!window.location.hash || window.location.hash === '' || window.location.hash === '#') {
      return;
    }

    // Step 2: Check if the URL's `#faq-id` is well formed, i.e., if it matches `/^#[a-z]([a-z0-9]|-[a-z0-9])*$/`.
    // If it is ill-formed, don't process any redirects;
    // Display `"Requested ID faq-id is ill-formed"`, then Stop.
    var targetId = window.location.hash.slice(1);
    if (idIsIllFormed(targetId)) {
      $msgbox('fail')
        .text('Sorry, the requested ' + idIsIllFormed(targetId))
        .mouseover();
      return;
    }

    // Step 3: Check if the page defines an anchor `<a id="faq-id">` with the same name as the URL's `#faq-id`.
    // If the target-anchor exists, the browser has the correct page for the target FAQ;
    // Stop successfully.
    if ($('#' + targetId).length) {
      return;
    }

    // Step 4: Display an info-msg: `"Locating FAQ <faq-id>"`, and do an ajax load of ID_DEFNS_URL.
    // Note: this `GET` *should* succeed even if the user isn't a FAQ Editor and/or isn't logged in.
    $msgbox('info')
      .text('Locating FAQ ' + targetId + ' ...')
      .mouseover();
    $.get(ID_DEFNS_URL)
      .fail(function(xhr, textStatus, errorThrown) {
        // If the `GET` fails, we can't locate the URL's FAQ;
        // Display a fail-msg: `"FAQ <faq-id> is temporarily unavailable; please try later"`, then Stop.
        $msgbox('fail')
          .html('Sorry, FAQ ' + targetId + ' is temporarily unavailable.<br>Please try again later.')
          .mouseover();
        messageToSiteAdmins({
          severity: 1,
          category: 'http-failure',
          itemId:   ID_DEFNS_ID,
          pathname: ID_DEFNS_URL,
          details:  'status ' + textStatus + ', error ' + errorThrown});
      })
      .done(function(idDefns) {
        // Step 5: Look in `id-defns` for the *`faq-id` defn line* (`[faq-id] /wiki/faq/correct-section#faq-id "faq descrip"`).
        // If the *`faq-id` defn line* is not in `id-defns`, the original URL had an unknown FAQ ID;
        // Display `"No such FAQ: faq-id"`, then Stop.
        var match = idDefns.match(new RegExp("^\\[" + targetId + "\\]: +(\\S+)", "m"));
        if (!match) {
          $msgbox('fail')
            .text('Sorry, no such FAQ: ' + targetId)
            .mouseover();
          return;
        }

        // Step 6: Check if the correct URL (from the *`faq-id` definition line*) is the same as what is already loaded in the browser.
        // If the browser already has the correct URL, then `id-defns` is broken (`id-defns` erroneously says `faq-id` is on the current page):
        // Alert the FAQ Admins that the `faq-id` line in `id-defns` is wrong,
        // Display `"FAQ faq-id is temporarily unavailable; please try later"`, then Stop.
        if (match[1].replace(/^.*\//, '').replace(/#.*$/, '').toLowerCase() ===
            window.location.pathname.replace(/^.*\//, '').toLowerCase()) {
          $msgbox('fail')
            .html('Sorry, FAQ ' + targetId + ' is temporarily unavailable.<br>Please try again later.')
            .mouseover();
          messageToSiteAdmins({
            severity: 1,
            category: 'faq-toc-is-broken',
            itemId:   ID_DEFNS_ID,
            pathname: ID_DEFNS_URL,
            details:  targetId + ' is not at ' + window.location.pathname + '#' + targetId});
          return;
        }

        // Step 7: Redirect the browser to the correct URL. This is the ultimate purpose of all the other steps.
        // Display `"Found FAQ faq-id; redirecting to correct-section#faq-id"` (in case the page-load fails).
        // Use `window.location.replace()` to load the correct URL (to discard the rotten URL from browser's history).
        $msgbox('good')
          .append('Found FAQ ' + targetId + '.<br>Redirecting to ',
                  $('<a/>', {href: match[1], text: match[1].replace(/^.*\//, '')}))
          .mouseover();
        window.location.replace(match[1]);

        // Note: step #7 restarts the procedure at step #1 in the new page. However an infinite loop cannot occur,
        // since the second iteration can never get past step #6 --- the second iteration uses a URL from `id-defns`.
      });
  };
  window.onhashchange();
  
  // FAQ suggestion popups
  $('.js-inline-popup').click(function(e) { e.preventDefault(); });
  $('.js-inline-popup').each(function() {
    var tooltipContent = $($(this).data('tooltip'));
    $(this).qtip({
      content: {
        text: tooltipContent
      },
      style: {
        classes: 'qtip-light'
      },
      position: {
        my: 'top left',  // Position my top left...
        at: 'center center'
      },
      show: {
        event: 'click'
      },
      hide: {
        event: 'unfocus'
      },
      suppress: false
    });
  });
  $('#contact_form').submit(function() {
    $(this).find('input[type="submit"]').val('Sending...').attr('disabled', 'disabled');
  });
});

// Swap out the Article Edit textarea contents
$("#btn-load-wiki-from-db").click(function (e) {
  e.preventDefault();
  $("#article_content").html($("#article_content_db").html());
  $("#article_content").css("background-color", "#dfd");
  $("#article_content").animate({
    backgroundColor: "#fff"
  }, 1000);
});
$("#btn-load-wiki-from-repo").click(function (e) {
  e.preventDefault();
  $("#article_content").html($("#article_content_repo").html());
  $("#article_content").css("background-color", "#dfd");
  $("#article_content").animate({
    backgroundColor: "#fff"
  }, 1000);
});