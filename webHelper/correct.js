/**
 * 批改页面交互
 */
$(function () {
	correctHomework.init();
});

var crtProgressObj = $(".selectbox-check-cont");
var correctAnswer = $(".hwfi_correct_answer");
//答题详情
var topicAnswerDetails = [];
var currentAnswerDetail = {};
//当前题目的索引位置
var topicIndex = 0;
var currentUser = {};
//当前学生的索引位置
var userIndex = 0;
var topicInfoDetails = {};
//是否使用“打分”模式
var isKeyScore = false;
//是否使用“对错班队”模式打分
var isRightOrWrong = false;
/**
 * 批改组件点击事件
 */
function correctBtnClick(input) {
	var scoreStr = $('#score_text').val();
	var score = topicInfoDetails[currentAnswerDetail.topicId].score;
	var regex = /[0-9+]/;
	if ('correct' == input) {
		//对错半对赋分
		markScoreType("rightOrWrong");
		scoreStr = score;
	} else if ('wrong' == input) {
		//对错半对赋分
		markScoreType("rightOrWrong");
		scoreStr = 0;
	} else if ('half' == input) {
		//对错半对赋分
		markScoreType("rightOrWrong");
		scoreStr = score / 2;
	} else if(''==input){
		scoreStr = '';
	}else if (scoreStr.indexOf('.5') != -1) {
		return;
	} else if (regex.test(input)) {
		if (parseFloat(scoreStr) > 0) {
			scoreStr += input;
		} else if ((scoreStr == ''|| scoreStr=='0') && '.5' == input) {
			scoreStr = 0.5;
		} else {
			scoreStr = input;
		}
		//直接打分赋分
		markScoreType("keyScore");
	}
	if (parseFloat(scoreStr) > score) {
		$('#score_text').css({
			"color": "red"
		});
	} else {
		$('#score_text').css({
			"color": "black"
		});
	}
	$('#score_text').val(scoreStr);
}

function markScoreType(str){
	if(str){
		if("keyScore" == str){
			//赋分标签
			isKeyScore = true;
			isRightOrWrong = false;
		}
		if("rightOrWrong" == str){
			//赋分标签
			isKeyScore = false;
			isRightOrWrong = true;
		}
	}
}
/**
 * 保存批改结果
 */
function saveCorrectResult() {
	//保存数据
	var topic = {
		id: currentAnswerDetail.topicId,
		seqNO: currentAnswerDetail.seqNO,
		isSubjective: true
	};
	//得分
	var markScoreVal = $("#score_text").val();
	if ('' == markScoreVal) {
		CommonDialog.tips('您还未批改，请批改后保存。');
		return;
	}
	var markScore = parseFloat(markScoreVal);
	var totalScore = topicInfoDetails[currentAnswerDetail.topicId].score;
	var answerDetail = {
		topic: topic,
		markScore: markScore
	};
	if (markScore > totalScore) {
		CommonDialog.tips('得分超过满分！');
		return;
	}
	if (markScore == totalScore) {
		answerDetail.resultType = 'CORRECT';
	} else if (markScore == 0) {
		answerDetail.resultType = 'WRONG';
	} else {
		answerDetail.resultType = 'HALFCORRECT'
	}

	//更新数据库批改信息
	$.ajax({
		"url": basePath + "/main/correct/saveCorrectResult/",
		"type": "POST",
		"data": {
			"userId": currentUser.userId,
			"homeworkId": homeworkId_tea,
			"topicPackId": topicPackId_tea,
			"answerDetail": JSON.stringify(answerDetail)
		},
		dataType: "json",
		"success": function (data) {
			//埋点记录赋分行为
			if(isKeyScore && !isRightOrWrong){
				userActionLog("zxzy_teacher","1012",currentUser.userId,null);
			}
			if(!isKeyScore && isRightOrWrong){
				userActionLog("zxzy_teacher","1011",currentUser.userId,null);
			}
			//修改缓存中的数据
			topicAnswerDetails[topicIndex].userInfos[userIndex].resultType = answerDetail.resultType;
			topicAnswerDetails[topicIndex].userInfos[userIndex].markScore = answerDetail.markScore;
			currentAnswerDetail = topicAnswerDetails[topicIndex];
			currentUser = topicAnswerDetails[topicIndex].userInfos[userIndex];
			//
			toNext();
		},
		"error": function () {
			CommonDialog.tips('数据提交失败！');
		}
	});

}

//to next
function toNext() {
	//页面刷新
	var userInfos = currentAnswerDetail.userInfos;
	var userLength = userInfos.length,
	topicLength = topicAnswerDetails.length,
	disableNext = false;
	if (userIndex == userLength - 1 && topicIndex == topicLength - 1) {
		return;
	}
	//如果当前学生是最后一个，则跳转到下一题的第一个学生
	if (userIndex == userLength - 1) {
		currentAnswerDetail = topicAnswerDetails[topicIndex + 1];
		currentUser = currentAnswerDetail.userInfos[0];

		userIndex = 0;
		topicIndex += 1;
	} else {
		currentUser = userInfos[userIndex + 1];
		userIndex += 1;
	}
	if (userIndex == userLength - 1 && topicIndex == topicLength - 1) {
		disableNext = true;
	}

	//刷新答题区域
	initAnswerArea(currentAnswerDetail.seqNO, currentAnswerDetail.topicId, currentUser);
	//刷新正确答案
	var topicInfoDetail = topicInfoDetails[currentAnswerDetail.topicId];
	$(".question_work .area").html('<span class="tit mt20">题目：</span>'+topicInfoDetail.rawHtml+'<br/>'+'<span class="tit mt20">正确答案：</span>'+topicInfoDetail.answerHtml);
	var scoreHtml = '<span class="fb col3">评分</span>' + ' （满分' + topicInfoDetail.score + '分）';
	$("#totalScore").html(scoreHtml);
	//禁用next按钮
	if (disableNext) {
		$("#next").removeAttr("href").removeClass("next").addClass("nextDis");
	}
	$("#pre").attr("href", "javascript:void(0);").removeClass("preDis").addClass("pre");
}

/**
 * 初始化答题区域
 */
function initAnswerArea(seqNO, topicId, userInfo) {
	var answer = JSON.parse(userInfo.answer);
	var htmls = [],
	_html = '';
	//
	if (typeof currentUser.markScore != 'undefined') {
		//				markScore = currentUsers[i].markScore + '分';
		//				_uHtml += '<a href="javascript:void(0);" class="check" data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '_' + currentUsers[i].userId + '"><span class="name">' + userName + ' </span><span class="score">  ' + markScore + '</span></a>'
		//初始化打分框
		$('#score_text').val(currentUser.markScore);
	} else {
		$('#score_text').val('');
	}

	//刷新标题
	$("#answer_title").text('第' + seqNO + '题-' + userInfo.userName);
	if (answer && answer.pic) {
		if (userInfo.answerComment) {
			var stuComment = eval('(' + userInfo.answerComment + ')');
			if (stuComment.pics) {
				//多图片展示
				for (var i = 0; i < stuComment.pics; i++) {
					if (stuComment.pics[i]) {
						_html = '<div style="position: relative;"><div class="showPics imgDiv_' + i + '" id="pic_' + seqNO + '_' + topicId + '"><img style="max-width: 822px;cursor: pointer;" id="pic_' + seqNO + '_' + topicId + '_' + i + '" ' +
							'src="' + stuComment.pics[i] + '"  data-pics=' + JSON.stringify(stuComment.pics) + ' ></div>'
							+ '<div style="position: absolute;top: 0;right: 0px;"><a id="rotate_box_' + i + '" href="javascript:;"><img src="/homework/public/module/global/pocket/teacher/images/hw_rotate.png"></img></a></div></div>';
						htmls.push(_html);
					}
				}
			}
		} else {
			//多图片展示
			for (var i = 0; i < answer.pic.length; i++) {
				_html = '<div style="position: relative;"><div class="showPics imgDiv_' + i + '" id="pic_' + seqNO + '_' + topicId + '"><img style="max-width: 822px;cursor: pointer;" id="pic_' + seqNO + '_' + topicId +'_'+i+ '" ' +
					'src="' + answer.pic[i].url + '"  data-pics=' + JSON.stringify(answer.pic) + ' ></div>'
					+'<div style="position: absolute;top: 0;right: 0px;"><a id="rotate_box_'+ i + '" href="javascript:;"><img src="/homework/public/module/global/pocket/teacher/images/hw_rotate.png"></img></a></div></div>';
				htmls.push(_html);
			}
		}
	}
	if (answer && answer.text) {
		_html = '  <div class="sum_add_edit textarea">' + answer.text + '</div>';
		htmls.push(_html);
	} else {
		if (answer && answer.offline) {
			_html = '  <div class="sum_add_edit textarea">线下作答</div>';
			htmls.push(_html);
		}
	}
	$(".paper").html(htmls.join(''));
	//图片操作展示
	showOperPics();
}

/**
 * 图片操作展示
 */
function showOperPics() {
	//图片展示
	$(".showPics").each(function () {
		var _this = $(this);
		var index = -1;
		$(this).off("click").click(function () {
			index = $(".showPics").index(_this);
			var pics = _this.find("img").attr("data-pics");
			if (pics && typeof pics == 'string') {
				pics = eval('(' + pics + ')');
			}

			var filterList = [];
			for (var i = 0; i < pics.length; i++) {
				if (pics[i].url) {
					filterList.push(pics[i].url);
				} else {
					filterList.push(pics[i]);
				}
			}
			$(".reply-pane").html(__picTmpl__({
					"picList": filterList
				}));
			//动态获取body对话框的大小
			$(".reply-pane .pic").css({
				"width": $("body").width(),
				"height": $(window).height()
			});

			//viewer.js配置信息
			var picOptions = {
				inline: true,
				keyboard: false,
				toolbar: false,
				navbar: false,
				button: false,
				title: false,
				zoomable: true,
				maxZoomRatio: 2,
				minZoomRatio: 0.5
			};
			//放大缩小展示
			$("#imagesUl").viewer(picOptions).viewer("view", index);
			$(".reply-pane").show();
			$(".pic").show();
			//图片查看器关闭
			$("#closedButton").click(function () {
				$(".reply-pane").hide();
			});
			//放大 缩小旋转事件
			$(".button").click(function () {
				var data = $(this).data();
				var args = data.arguments || [];

				if (data.method) {
					$("#imagesUl").viewer(picOptions).viewer(data.method, args[0], args[1]);
				}

				//拖动图片
				$('#banner').unbind().mousedown(
					function (event) {
					var isMove = true;
					var abs_x = event.pageX - $('#moveBar').offset().left;
					var abs_y = event.pageY - $('#moveBar').offset().top;
					$(document).mousemove(function (event) {
						if (isMove) {
							var obj = $('#moveBar');
							obj.css({
								'left': event.pageX - abs_x,
								'top': event.pageY - abs_y
							});
						}
					}).mouseup(
						function () {
						isMove = false;
					});
				});

			});
		});
		
		//图片旋转
		var rotate = null;
		$(this).next().off("click").click(function() {
			//绑定旋转事件
        	var obj = _this.find("img").attr("id");
        	if(null == rotate) {
        		rotate = new ImgRotate(obj);
        	}
        	rotate.initRotate(90);
		});
	});
	
	
	
}

var correctHomework = (function () {
	/**
	 * 初始化答题详情
	 */
	function initTopicAnswerDetails() {
		//先取一个学生的答题情况
		var topicPackStuStat = topicPackStuStats[0];
		var topicDetails = topicPackStuStat.deatils;
		for (var i = 0; i < topicDetails.length; i++) {
			//如果是主观题
			if (topicDetails[i].topic.isSubjective) {
				var topicAnswerDetail = {};
				topicAnswerDetail.seqNO = topicDetails[i].topic.seqNO;
				topicAnswerDetail.topicId = topicDetails[i].topic.id;
				var userInfos = [];
				var userInfo = {};
				userInfo.answer = topicDetails[i].answer;
				userInfo.resultType = topicDetails[i].resultType;
				//如果改题已批改，则该题有分
				if (userInfo.resultType != 'UNKNOWN') {
					userInfo.markScore = topicDetails[i].markScore;
				}
				userInfo.userId = topicPackStuStat.userId;
				userInfo.userName = topicPackStuStat.userName;
				userInfo.answerComment = topicPackStuStat.comment;
				userInfos.push(userInfo);
				topicAnswerDetail.userInfos = userInfos;
				topicAnswerDetails.push(topicAnswerDetail);
			}
		}
		//答题详情
		for (var i = 1; i < topicPackStuStats.length; i++) {
			for (var j = 0; j < topicPackStuStats[i].deatils.length; j++) {
				var topicDetail = topicPackStuStats[i].deatils[j];
				for (var k = 0; k < topicAnswerDetails.length; k++) {
					if (topicDetail.topic.seqNO == topicAnswerDetails[k].seqNO) {
						var tpcDetail = topicAnswerDetails[k];
						var userInfo = {};
						userInfo.answer = topicDetail.answer;
						userInfo.resultType = topicDetail.resultType;
						//如果改题已批改，则该题有分
						if (userInfo.resultType != 'UNKNOWN') {
							userInfo.markScore = topicDetail.markScore;
						}
						userInfo.userId = topicPackStuStats[i].userId;
						userInfo.userName = topicPackStuStats[i].userName;
						userInfo.answerComment = topicPackStuStat.comment;
						tpcDetail.userInfos.push(userInfo);
					}
				}
			}
		}

		//题目详情信息
		for (var i = 0; i < paperInfo.length; i++) {
			var topicInfoDetail = paperInfo[i].topicDetail;
			var topicId = topicInfoDetail.topicId;
			if (topicInfoDetail.section.isSubjective) {
				topicInfoDetails[topicId] = topicInfoDetail;
			}
		}
		// 		var jsonStr = JSON.stringify(topicAnswerDetails);
		// 		console.log(jsonStr);
	}

	/**
	 * 初始化静态页面
	 */
	function initStaticPage() {
		var topicsHtmls = [],
		usersHtmls = [],
		notCorrected = false;
		for (var i = 0; i < topicAnswerDetails.length; i++) {
			var _t_html = '',
			_uHtml = '';
			var seqNO = topicAnswerDetails[i].seqNO;
			var topicId = topicAnswerDetails[i].topicId;
			_t_html = '<dd><span data-id="' + topicId + '_' + seqNO + '">第' + seqNO + '题</span><i></i></dd>';

			//取第一个没有全部批改的题目
			if (!notCorrected) {
				var userInfos = topicAnswerDetails[i].userInfos;
				for (var j = 0; j < userInfos.length; j++) {
					var resultType = userInfos[j].resultType;
					if (resultType == 'UNKNOWN') {
						//当前索引
						if(!notCorrected) {
							topicIndex = i;
							userIndex = j;
							currentAnswerDetail = topicAnswerDetails[i];
							currentUser = userInfos[j];
						}
						notCorrected = true;
						_t_html = '<dd class="thistab"><span data-id="' + topicId + '_' + seqNO + '">第' + seqNO + '题</span><i></i></dd>';
					}
				}
			}
			topicsHtmls.push(_t_html);
		}
		//如果全部完了，则取最后一道题目,最后一个学生
		if (!notCorrected) {
			currentAnswerDetail = topicAnswerDetails[topicAnswerDetails.length - 1];
			currentUser = currentAnswerDetail.userInfos && currentAnswerDetail.userInfos[currentAnswerDetail.userInfos.length - 1];
			//当前索引
			topicIndex = topicAnswerDetails.length - 1;
			userIndex = currentAnswerDetail.userInfos.length - 1;

			topicsHtmls.pop();
			_html = '<dd class="thistab"><span data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '">第' + seqNO + '题</span><i></i></dd>';
			topicsHtmls.push(_t_html);
		}
		var currentUsers = currentAnswerDetail.userInfos;
		_uHtml += '<li class="clearfix">';
		for (var i = 0; i < currentUsers.length; i++) {
			var userName = currentUsers[i].userName,
			markScore = '';
			if (typeof currentUsers[i].markScore != 'undefined') {
				markScore = currentUsers[i].markScore + '分';
				_uHtml += '<a href="javascript:void(0);" class="check" data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '_' + currentUsers[i].userId + '"><span class="name">' + userName + ' </span><span class="score">  ' + markScore + '</span></a>'
				//初始化打分框
				$('#score_text').val(currentUsers[i].markScore);
			} else {
				_uHtml += '<a href="javascript:void(0);" class="uncheck" data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '_' + currentUsers[i].userId + '"><span class="name">' + userName + ' </span><span class="score">  ' + markScore + '</span></a>'
				$('#score_text').val('');
			}
		}
		_uHtml += '</li>';
		//初始化页面
		$(".sc-bd").html(topicsHtmls.join(''));
		$(".sc-ft").html(_uHtml);
		initAnswerArea(currentAnswerDetail.seqNO, currentAnswerDetail.topicId, currentUser);
		var topicInfoDetail = topicInfoDetails[currentAnswerDetail.topicId];
		$(".question_work .area").html('<span class="tit mt20">题目：</span>'+topicInfoDetail.rawHtml+'<br/>'+'<span class="tit mt20">正确答案：</span>'+topicInfoDetail.answerHtml);
		$(".question_work").hide();
		var scoreHtml = '<span class="fb col3">评分</span>' + ' （满分' + topicInfoDetail.score + '分）';
		$("#totalScore").html(scoreHtml);
		//初始化next/pre按钮
		if (topicIndex == 0 && userIndex == 0) {
			$("#pre").removeAttr("href").removeClass("pre").addClass("preDis");
		} else if (topicIndex == topicAnswerDetails.length - 1 && userIndex == currentAnswerDetail.userInfos.length - 1) {
			$("#next").removeAttr("href").removeClass("next").addClass("nextDis");
		}
	}

	/**
	 * 初始化页面事件
	 */
	function initPageEvent() {
		var timer1;
		var timer2;
		function ac1() {
			$(".selectbox-check .hd").find("i").addClass("up");
			$(".selectbox-check-cont").show();

			if ($(".sc-ft").height() < $(".sc-bd").height()) {
				$(".sc-ft").height($(".sc-bd").height() + 26);
			};
		};

		function ac2() {
			$(".selectbox-check .hd").find("i").removeClass("up");
			$(".selectbox-check-cont").hide();
		};

		//批改进度
		$(".selectbox-check").hover(function () {
			timer1 = setTimeout(ac1, 100);
			clearTimeout(timer2);
		}, function () {
			timer2 = setTimeout(ac2, 100);
			clearTimeout(timer1);
		});
		//答案展示
		answerDisplayEvent();
		//题目点击事件
		topicSelectedEvent();
		//学生名单点击事件
		userSelectedEvent();

		/**
		 * “上一个”点击事件
		 */
		$("#pre").off("click").click(function () {
			var userInfos = currentAnswerDetail.userInfos;
			var userLength = userInfos.length,
			topicLength = topicAnswerDetails.length,
			disablePre = false;
			if (userIndex == 0 && topicIndex == 0) {
				return;
			}
			//如果当前学生是第一个，则跳转到上一题的最后一个学生
			if (userIndex == 0) {
				currentAnswerDetail = topicAnswerDetails[topicIndex - 1];
				currentUser = currentAnswerDetail.userInfos[userLength - 1];

				topicIndex -= 1;
				userIndex = userLength - 1;

			} else {
				currentUser = userInfos[userIndex - 1];
				userIndex -= 1;
			}
			if (userIndex == 0 && topicIndex == 0) {
				disablePre = true;
			}

			//刷新答题区域
			initAnswerArea(currentAnswerDetail.seqNO, currentAnswerDetail.topicId, currentUser);
			//刷新正确答案
			var topicInfoDetail = topicInfoDetails[currentAnswerDetail.topicId];
			$(".question_work .area").html('<span class="tit mt20">题目：</span>'+topicInfoDetail.rawHtml+'<br/>'+'<span class="tit mt20">正确答案：</span>'+topicInfoDetail.answerHtml);
			var scoreHtml = '<span class="fb col3">评分</span>' + ' （满分' + topicInfoDetail.score + '分）';
			$("#totalScore").html(scoreHtml);
			//禁用上一个按钮
			if (disablePre) {
				$("#pre").removeAttr("href").removeClass("pre").addClass("preDis");
			}
			$("#next").attr("href", "javascript:void(0);").removeClass("nextDis").addClass("next");

		});

		/**
		 * 下一个点击事件
		 */
		$("#next").off("click").click(function () {
			//得分
			var markScoreVal = $("#score_text").val();
			if ('' == markScoreVal) {
				toNext();
			} else {
				saveCorrectResult();
			}
		});

		//提交按钮点击事件
		$(".submit").off("click").click(function () {
//			for (var i = 0; i < topicAnswerDetails.length; i++) {
//				var userInfos = topicAnswerDetails[i].userInfos;
//				for (var j = 0; j < userInfos.length; j++) {
//					if (userInfos[j].resultType == 'UNKNOWN') {
//						var d = art.dialog({
//								title: "提示",
//								content: "<p style='font-size: 16px;'>您尚有题目未批改，不能提交。</p>",
//								backdropBackground: "gray",
//								backdropOpacity: 0.3,
//								ok: function () {},
//								okValue: '确定',
//								width: 300,
//								height: 150,
//								drag: true,
//								fixed: true,
//								lock: true
//							});
//						return;
//					}
//
//				}
//			}

			//用户id列表
			var userIds = [];
			for (var i = 0; i < currentAnswerDetail.userInfos.length; i++) {
				userIds.push(currentAnswerDetail.userInfos[i].userId);
			}
			$.ajax({
				"url": basePath + "/main/correct/submitCorrectResult/",
				"type": "POST",
				"data": {
					"stdIds": JSON.stringify(userIds),
					"homeworkId": homeworkId_tea,
					"topicPackId": topicPackId_tea
				},
				dataType: "json",
				"success": function () {
					window.history.back();
				},
				"error": function () {
					CommonDialog.tips('数据提交失败！');
				}
			});

		});

	}

	/**
	 * 答案展示事件
	 */
	function answerDisplayEvent() {
		//答案显示
		var i = 0;
		$(".show-answer").off("click").click(function () {
			if (0 == i) {
				$("#answer").css({
					"display": "block"
				});
				$(".question_work").show();
				i = 1;
			} else {
				$("#answer").css({
					"display": "none"
				});
				$(".question_work").hide();
				i = 0;
			}
		});
	}

	/**
	 * 题目选中事件
	 */
	function topicSelectedEvent() {
		$(".sc-bd").find("dd").each(function () {
			var _this = $(this);
			$(this).off("click").click(function () {
				_this.addClass("thistab").siblings("dd").removeClass("thistab");
				var currentUserInfo = null;
				var dataId = _this.find("span").attr("data-id");
				var topicInfo = dataId.split('_');
				var topicId = topicInfo[0],
				seqNO = topicInfo[1];
				for (var i = 0; i < topicAnswerDetails.length; i++) {
					if (topicId == topicAnswerDetails[i].topicId && seqNO == topicAnswerDetails[i].seqNO) {
						//当前选中的题目
						currentAnswerDetail = topicAnswerDetails[i];
						//当前题目位置
						topicIndex = i;
						var userInfos = topicAnswerDetails[i].userInfos;
						var htmls = [];
						var _uHtml = '<li class="clearfix">';
						for (var i = 0; i < userInfos.length; i++) {
							var _html = '';
							var userName = userInfos[i].userName;
							var markScore = '';
							if (typeof userInfos[i].markScore != 'undefined') {
								markScore = userInfos[i].markScore + '分';
								_uHtml += '<a href="javascript:void(0);" class="check" data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '_' + userInfos[i].userId + '"><span class="name">' + userName + ' </span><span class="score">  ' + markScore + '</span></a>'
								//初始化打分框
								$('#score_text').val(userInfos[i].markScore);
							} else {
								_uHtml += '<a href="javascript:void(0);" class="uncheck" data-id="' + currentAnswerDetail.topicId + '_' + currentAnswerDetail.seqNO + '_' + userInfos[i].userId + '"><span class="name">' + userName + ' </span><span class="score">  ' + markScore + '</span></a>'
							}
						}
						_uHtml += '</li>';
						$(".sc-ft").html(_uHtml);
						//初始化点击事件
						userSelectedEvent();
						//正确答案
						var topicInfoDetail = topicInfoDetails[topicId];
						$(".question_work .area").html('<span class="tit mt20">题目：</span>'+topicInfoDetail.rawHtml+'<br/>'+'<span class="tit mt20">正确答案：</span>'+topicInfoDetail.answerHtml);
						var scoreHtml = '<span class="fb col3">评分</span>' + ' （满分' + topicInfoDetail.score + '分）';
						$("#totalScore").html(scoreHtml);
						break;
					}
				}
			});
		});
	}

	/**
	 * 学生名单点击事件
	 */
	function userSelectedEvent() {
		$(".sc-ft").find("a").each(function () {
			var _this = $(this);
			$(this).off("click").click(function () {
				var dataId = _this.attr("data-id");
				var userInfo = dataId.split('_');
				var topicId = userInfo[0],
				seqNO = userInfo[1],
				userId = userInfo[2];
				for (var i = 0; i < topicAnswerDetails.length; i++) {
					if (topicId == topicAnswerDetails[i].topicId && seqNO == topicAnswerDetails[i].seqNO) {
						var userInfos = topicAnswerDetails[i].userInfos;
						for (var j = 0; j < userInfos.length; j++) {
							var userInfo = userInfos[j];
							if (userId == userInfo.userId) {
								currentUser = userInfo;
								//用户位置
								userIndex = j;
								//答案区域
								initAnswerArea(seqNO, topicId, userInfo);
							}
						}
					}
				}
				//添加选中样式
				_this.siblings().removeClass("on");
				_this.addClass("on");
				//初始化next/pre按钮
				$("#next").attr("href", "javascript:void(0);").removeClass("nextDis").addClass("next");
				$("#pre").attr("href", "javascript:void(0);").removeClass("preDis").addClass("pre");
				if (topicIndex == 0 && userIndex == 0) {
					$("#pre").removeAttr("href").removeClass("pre").addClass("preDis");
				}
				if (topicIndex == topicAnswerDetails.length - 1 && userIndex == currentAnswerDetail.userInfos.length - 1) {
					$("#next").removeAttr("href").removeClass("next").addClass("nextDis");
				}
			});
		});
	}

	return {
		init: function () {
			$(".reply-pane").hide();
			initTopicAnswerDetails();
			initStaticPage();
			initPageEvent();
		}
	}

})();