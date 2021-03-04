// Initialize JSPrettify for code samples
window.jsprettify.run();

// Replace Twitter widget footer text once tweets are loaded
var twtrFtr = setInterval(checkTwitter, 100);
function checkTwitter() {
  var h = $("div.twtr-hd h4 a");
  var f = $("a.twtr-join-conv");
  if (h.length > 0 && f.length > 0) {
    $(".twtr-tweet-text p").each(function(index, element) {
      // Strip out everything but the tweet content
      var aUser = $(this).find(".twtr-user");
      $(this).find(".twtr-user").remove();
      var em = $(this).find("em");
      $(this).find("em").remove();
      var aBitly = $(this).find("a");
      $(this).find("a").remove();
      
      // Decode any HTML entities in tweet content
      var decoded = $('<div/>').html($(this).html()).text();
      
      // Reconstruct the tweet item
      $(this).html("");
      $(this).append(aUser);
      $(this).append(decoded);
      if (aBitly != null && aBitly != undefined) {
        $(this).append(aBitly);
      }
      $(this).append(em);
    });
    f.text("Follow @" + h.text());
    clearInterval(twtrFtr);
  }
}

$(document).ready(function() {
  
  // Highlight all <pre> and <code> elements as C++ code
  $("pre").addClass("prettyprint linenums lang-cpp");
  prettyPrint();

  // Initialize offline reading links
  if ($("a.instapaper").length > 0) {
    $("a.instapaper").attr("href", $("a.instapaper").attr("href").replace("{url}", encodeURIComponent(document.URL)));
  }
  if ($("a.pocket").length > 0) {
    $("a.pocket").attr("href", $("a.pocket").attr("href").replace("{url}", encodeURIComponent(document.URL)));
  }
  
  // Toggle the Readability toolbar
  $("a.icon.readability").click(function(e) {
    e.preventDefault();
    $(this).hide();
    $(this).siblings(".rdbWrapper").show();
  });
  
  // Prevent Esc key from clearing changes on Wiki edit form
  $("#article_content").keydown(function(e) {
    if (e.keyCode == 27) return false;
  });

});



var soTags = ['c%2b%2b', 'c%2b%2b11', 'c%2b%2b14', 'c%2b%2b1z', 'c%2b%2b17'];
var soTagsFetched = 0;
var soLinks = [];

// Retrieve questions from Stack Overflow
$(document).ready(function() {
  if ($('#stackoverflow_cpp').length > 0) {
    for (var i = 0; i < soTags.length; i++) {
      fetchStackOverflowLinks(soTags[i]);
    }
  }
});

function fetchStackOverflowLinks(tag) {
  var url = 'https://api.stackexchange.com/2.1/questions?key=31KxHRpdst0VZ4q6w6qLcQ((&page=1&pagesize=100&sort=creation&order=desc';
  var maxresults = 6;
  var min_score = 4;
  var min_answers = 1;

  $.ajax({
      dataType: 'jsonp',
      jsonp: 'jsonp',
      url: url + '&site=stackoverflow&tagged=' + tag,
      success: function(data) {
        var cpp_count = 0;
        $.each(data.items, function(index, item) {
          if (cpp_count < maxresults && item.score >= min_score && item.answer_count >= min_answers) {
            var item_date = new Date(item.creation_date * 1000);
            var item_html = '<li>';
            item_html += '<a href="' + item.link + '" class="title">' + item.title + '</a>';
            item_html += '<a href="' + item.link + '" class="byline">By ' + item.owner.display_name + ' | ' + item_date.format('mmm d, yyyy h:MM TT') + '</a>';
            item_html += '</li>';
            cpp_count += 1;

            soLinks.push({ date: item.creation_date, html: item_html });
          }
        });

        soTagsFetched += 1;
        if (soTagsFetched == soTags.length) {
          renderStackOverflowLinks();
        }
      },
      error: function(val) {
        console.log('error');
        console.log(arguments);
      }
    });
}

function renderStackOverflowLinks() {
  soLinks = soLinks.sort(function(a, b) {
    return parseFloat(b.date) - parseFloat(a.date);
  });

  var soDistinctHtml = [];
  $.each(soLinks, function(index, value) {
    if ($.inArray(value.html, soDistinctHtml) === -1) {
      soDistinctHtml.push(value.html);
    }
  });

  var max = soDistinctHtml.length;
  if (max > 6) {
    max = 6;
  }

  var out_html = '';
  for (var i = 0; i < max; i++) {
    out_html += soDistinctHtml[i];
  }
  $('#stackoverflow_cpp').html(out_html);
}